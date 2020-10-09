#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/libata.h>

#include "hook.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rhydon");

#pragma GCC optimize("-fno-optimize-sibling-calls")

typedef void(*real_ata_qc_issue)(struct ata_queued_cmd* qc);

real_ata_qc_issue func;

static void fh_ata_qc_issue(struct ata_queued_cmd* qc) {
    printk("ATA command is: 0x%X\n", qc->tf.command);
    func(qc);
}

struct ftrace_hook hook;

static int ata_sniffer_init(void) {	
    printk(KERN_ALERT "hello...\n");
    func = (real_ata_qc_issue)kallsyms_lookup_name("ata_qc_issue");
    hook.name = "ata_qc_issue";
    hook.function = fh_ata_qc_issue;
    hook.original = &func;
    fh_install_hook(&hook);
    return 0;
}

static void ata_sniffer_exit(void) {
    printk(KERN_ALERT "bye...\n");
}

module_init(ata_sniffer_init);
module_exit(ata_sniffer_exit);