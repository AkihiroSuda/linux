// SPDX-License-Identifier: GPL-2.0-only

#include <linux/cred.h>
#include <linux/group_range.h>
#include <linux/uidgid.h>
#include <linux/seqlock.h>
#include <linux/sysctl.h>

static void get_group_range(struct group_range *gr, kgid_t *low, kgid_t *high)
{
	unsigned int seq;

	do {
		seq = read_seqbegin(&gr->lock);

		*low = gr->range[0];
		*high = gr->range[1];
	} while (read_seqretry(&gr->lock, seq));
}

static void set_group_range(struct group_range *gr, kgid_t low, kgid_t high)
{
	write_seqlock(&gr->lock);
	gr->range[0] = low;
	gr->range[1] = high;
	write_sequnlock(&gr->lock);
}

static int group_range_min[] = { 0, 0 };
static int group_range_max[] = { GROUP_RANGE_MAX, GROUP_RANGE_MAX };

int sysctl_group_range(sysctl_group_range_func_t fn, struct ctl_table *table,
				 int write, void *buffer, size_t *lenp, loff_t *ppos)
{
	struct group_range *gr = fn(table);
	struct user_namespace *user_ns = current_user_ns();
	int ret;
	gid_t urange[2];
	kgid_t low, high;
	struct ctl_table tmp = {
		.data = &urange,
		.maxlen = sizeof(urange),
		.mode = table->mode,
		.extra1 = &group_range_min,
		.extra2 = &group_range_max,
	};

	get_group_range(gr, &low, &high);
	urange[0] = from_kgid_munged(user_ns, low);
	urange[1] = from_kgid_munged(user_ns, high);
	ret = proc_dointvec_minmax(&tmp, write, buffer, lenp, ppos);

	if (write && ret == 0) {
		low = make_kgid(user_ns, urange[0]);
		high = make_kgid(user_ns, urange[1]);
		if (!gid_valid(low) || !gid_valid(high))
			return -EINVAL;
		if (urange[1] < urange[0] || gid_lt(high, low)) {
			low = make_kgid(&init_user_ns, 1);
			high = make_kgid(&init_user_ns, 0);
		}
		set_group_range(gr, low, high);
	}

	return ret;
}

bool check_current_group_range(struct group_range *gr)
{
	kgid_t group = current_egid();
	struct group_info *group_info;
	int i;
	kgid_t low, high;
	bool ret = true;

	get_group_range(gr, &low, &high);
	if (gid_lte(low, group) && gid_lte(group, high))
		return true;

	group_info = get_current_groups();
	for (i = 0; i < group_info->ngroups; i++) {
		kgid_t gid = group_info->gid[i];

		if (gid_lte(low, gid) && gid_lte(gid, high))
			goto out_release_group;
	}
	ret = false;
out_release_group:
	put_group_info(group_info);
	return ret;
}
