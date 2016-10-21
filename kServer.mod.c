#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc7a6b700, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x1e4e91ea, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xb053afc3, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xead012de, __VMLINUX_SYMBOL_STR(sock_create) },
	{ 0xa427d72c, __VMLINUX_SYMBOL_STR(sock_sendmsg) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0x92c05456, __VMLINUX_SYMBOL_STR(iov_iter_init) },
	{ 0x50eedeb8, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb4390f9a, __VMLINUX_SYMBOL_STR(mcount) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2BA8CAAAE85A0284C2DCEAE");
