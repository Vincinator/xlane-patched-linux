#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <asgard_con/asgard_con.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Distributed Systems Group");
MODULE_DESCRIPTION("ASGARD MLX5 Connection");
MODULE_VERSION("0.01");

/* Initialized in asgard_mlx5_con_init*/
static struct asgard_mlx5_con_info **infos;
static int mlx5_devices;

int get_asgard_id_by_ifindex(int ifindex)
{
	int i;

	for(i = 0; i < mlx5_devices; i++) {
		if(infos[i]->ifindex == ifindex)
			return i;
	}
	return -1;
}
EXPORT_SYMBOL(get_asgard_id_by_ifindex);


int register_asgard_at_nic(int ifindex,
				void (*asgard_post_ts)(int, uint64_t, int),
				void (*asgard_post_payload)(int, void *, u16, u32),
				void (*asgard_force_quite)(void))
{
	int asgard_id;

	asgard_id = get_asgard_id_by_ifindex(ifindex);

	if(asgard_id < 0 ||asgard_id > ASGARD_MLX5_DEVICES_LIMIT ) {
		printk(KERN_ERR"Could not get asgard id by ifindex %d\n", asgard_id);
		return -ENODEV;
	}

	if(!infos[asgard_id])
		return -ENODEV;

	infos[asgard_id]->asgard_post_ts = asgard_post_ts;
	infos[asgard_id]->asgard_post_payload = asgard_post_payload;
	infos[asgard_id]->asgard_force_quit = asgard_force_quite;

	infos[asgard_id]->asgard_alive = 1;

	return 0;
}
EXPORT_SYMBOL(register_asgard_at_nic);


int asgard_mlx5_con_unregister_device(int ifindex)
{
	int asgard_id;


	if(infos == NULL){
		printk(KERN_ERR "[ASGARD] Connection Info is not setup..\n");
		return -1;
	}

	printk(KERN_INFO "[ASGARD] Unregister mlx5 device with ifindex %d\n", ifindex, __FUNCTION__);

	asgard_id = get_asgard_id_by_ifindex(ifindex);

	if(asgard_id < 0) {
		printk(KERN_INFO "[ASGARD] Got invalid asgard id (%d) for ifindex (%d) in function %s\n", asgard_id, ifindex, __FUNCTION__);
		return -1;
	}

	if (!infos[asgard_id]) {
		printk(KERN_INFO "[ASGARD] Allocation error in function %s\n", __FUNCTION__);
		return -1;
	}

	if(infos[asgard_id]->asgard_alive) {
		printk(KERN_INFO "[ASGARD] Asgard Kernel Module is still running - trying to force quit asgard now.. \n");

		if(infos[asgard_id]->asgard_force_quit)
			infos[asgard_id]->asgard_force_quit();
		else
			printk(KERN_INFO "[ASGARD] Force Quit Function was not set by ASGARD! \n");
	}

	kfree(infos[asgard_id]);

	mlx5_devices--;

	return 0;

}

int asgard_mlx5_con_register_device(int ifindex)
{
	// int asgard_id = asgard_core_register_nic(ifindex);
	int asgard_id;

	if(infos == NULL){
		printk(KERN_ERR"[ASGARD] Connection Info is not setup..\n");
		return -1;
	}

	printk(KERN_INFO"[ASGARD] Registering new mlx5 device with ifindex %d -  in function %s\n", ifindex, __FUNCTION__);

	asgard_id = asgard_generate_next_id();

	printk(KERN_INFO"[ASGARD] Assigned asgard_id %d to new mlx5 device - in function %s\n", asgard_id, __FUNCTION__);

	if (asgard_id < 0)
		return -1;

	infos[asgard_id] =
		kmalloc(sizeof(struct asgard_mlx5_con_info), GFP_KERNEL);

	if (!infos[asgard_id]) {
		printk(KERN_INFO "[ASGARD] Allocation error in function %s\n", __FUNCTION__);
		return -1;
	}

	infos[asgard_id]->ix[0] = 999999;
	infos[asgard_id]->ix[1] = 999999;
	infos[asgard_id]->ix[2] = 999999;

	infos[asgard_id]->cqn[0] = 999999;
	infos[asgard_id]->cqn[1] = 999999;
	infos[asgard_id]->cqn[2] = 999999;


	infos[asgard_id]->ifindex = ifindex;
	infos[asgard_id]->asgard_alive = 0;

	mlx5_devices++;

	printk(KERN_INFO "[ASGARD] Register MLX5 Device with ifindex=%d\n", ifindex);

	printk(KERN_INFO "[ASGARD] Assigned asgard_id (%d) to ifindex (%d)\n", asgard_id,
		  ifindex);

	return asgard_id;
}
EXPORT_SYMBOL(asgard_mlx5_con_register_device);

void asgard_mlx5_post_optimistical_timestamp(int asgard_id, uint64_t cycle_ts, int ctype)
{
	// safe ctype check via asgard_mlx5_con_check_cqn
	if(ctype == 0)
		return;

	if(infos[asgard_id]->asgard_alive == 0)
		return;

	infos[asgard_id]->asgard_post_ts(asgard_id, cycle_ts, ctype);

	return;
}
EXPORT_SYMBOL(asgard_mlx5_post_optimistical_timestamp);

bool asgard_mlx5_post_payload(int asgard_id, void *va, u32 frag_size, u16 headroom,
			    u32 cqe_bcnt, int ctype)
{
	u8 *payload = (u8 *)va;


	if(ctype == 0)
		return false;
	
	if(infos[asgard_id]->asgard_alive == 0)
		return false;


	if(infos[asgard_id]->asgard_alive == 0)
		return false;

	infos[asgard_id]->asgard_post_payload(asgard_id, payload, headroom, cqe_bcnt);
	
	return true;
}
EXPORT_SYMBOL(asgard_mlx5_post_payload);

int asgard_mlx5_con_check_ix(int asgard_id, int ix)
{
	if (asgard_id < 0 || asgard_id >= ASGARD_MLX5_DEVICES_LIMIT) {
		printk(KERN_INFO "[ASGARD] asgard_id was %d in %s\n", asgard_id, __FUNCTION__);
		return 0;
	}

	if (!infos[asgard_id]) {
		printk(KERN_INFO "[ASGARD] invalid infos element at index %d in function %s\n", asgard_id, __FUNCTION__);
		return 0;
	}

	if (infos[asgard_id]->ix[0] == ix)
		return 1;

	if (infos[asgard_id]->ix[1] == ix)
		return 2;
	
	if (infos[asgard_id]->ix[2] == ix)
		return 3;

	return 0;
}
EXPORT_SYMBOL(asgard_mlx5_con_check_ix);

int asgard_mlx5_con_check_cqn(int asgard_id, u32 cqn)
{
	if (asgard_id < 0 || asgard_id >= ASGARD_MLX5_DEVICES_LIMIT) {
		printk(KERN_INFO "[ASGARD] asgard_id was %d in %s\n", asgard_id, __FUNCTION__);
		return 0;
	}

	if (!infos[asgard_id]) {
		printk(KERN_INFO "[ASGARD] invalid infos element at index %d in function %s\n", asgard_id, __FUNCTION__);
		return 0;
	}

	if (infos[asgard_id]->cqn[0] == cqn)
		return 1;

	if (infos[asgard_id]->cqn[1] == cqn)
		return 2;

	if (infos[asgard_id]->cqn[2] == cqn)
		return 3;

	return 0;

}
EXPORT_SYMBOL(asgard_mlx5_con_check_cqn);

int asgard_mlx5_con_register_channel(int asgard_id, int ix, u32 cqn, void *c, int asgard_channel_type)
{

	if (asgard_id < 0 || asgard_id >= ASGARD_MLX5_DEVICES_LIMIT) {
		printk(KERN_INFO "[ASGARD] asgard_id was %d in %s\n", asgard_id, __FUNCTION__);
		return 0;
	}

	if(asgard_channel_type <= 0||asgard_channel_type > 10) {
		printk(KERN_INFO"[ASGARD] invalid asgard channel type %d\n", asgard_channel_type);
		return 0;
	}

	//infos[asgard_id]->ctype = asgard_channel_type;
	infos[asgard_id]->ix[asgard_channel_type - 1 ] = ix;
	infos[asgard_id]->cqn[asgard_channel_type - 1] = cqn;
	infos[asgard_id]->c[asgard_channel_type - 1] = c;

	printk(KERN_INFO "[ASGARD] Channel registered with ix=%d, cqn=%d, channel Type=%d", ix, cqn, asgard_channel_type);

	return 0;
}
EXPORT_SYMBOL(asgard_mlx5_con_register_channel);

void unregister_asgard(void)
{
	int i;

	for (i = 0; i < mlx5_devices; i++) {
		infos[i]->asgard_post_ts = NULL;
		infos[i]->asgard_post_payload = NULL;
		infos[i]->asgard_force_quit = NULL;

		infos[i]->asgard_alive = 0;
	}
}
EXPORT_SYMBOL(unregister_asgard);

static int __init asgard_mlx5_con_init(void)
{
	int i;

	infos = kmalloc_array(ASGARD_MLX5_DEVICES_LIMIT,
			      sizeof(struct asgard_mlx5_con_info *), GFP_KERNEL);

	for (i = 0; i < ASGARD_MLX5_DEVICES_LIMIT; i++) {
		infos[i] = kmalloc(sizeof(struct asgard_mlx5_con_info), GFP_KERNEL);
	}
	return 0;
}

static void __exit asgard_mlx5_con_exit(void)
{
	int i;

	for (i = 0; i < mlx5_devices; i++)
		kfree(infos[i]);

	kfree(infos);
}

module_init(asgard_mlx5_con_init);
module_exit(asgard_mlx5_con_exit);
