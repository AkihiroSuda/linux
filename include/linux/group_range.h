/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_GROUP_RANGE_H
#define _LINUX_GROUP_RANGE_H

#include <linux/seqlock.h>
#include <linux/uidgid.h>

/*
 * gid_t is either uint or ushort.  We want to pass it to
 * proc_dointvec_minmax(), so it must not be larger than MAX_INT
 */
#define GROUP_RANGE_MAX (((gid_t)~0U) >> 1)

struct group_range {
	seqlock_t       lock;
	kgid_t          range[2];
};

typedef struct group_range* (*sysctl_group_range_func_t)(struct ctl_table *);
int sysctl_group_range(sysctl_group_range_func_t fn, struct ctl_table *table,
				 int write, void *buffer, size_t *lenp, loff_t *ppos);

bool check_current_group_range(struct group_range *gr);
#endif /* _LINUX_GROUP_RANGE_H */
