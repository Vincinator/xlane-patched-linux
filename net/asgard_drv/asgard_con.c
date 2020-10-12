#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include <asgard_con/asgard_con.h>

static int device_counter;

int asgard_generate_next_id(void)
{
	if (device_counter >= SASSY_MLX5_DEVICES_LIMIT) {
		printk(KERN_INFO
			"Reached Limit of maximum connected mlx5 devices.\n");
		printk(KERN_INFO "Limit=%d, device_counter=%d\n",
			    SASSY_MLX5_DEVICES_LIMIT, device_counter);
		return -1;
	}

	return device_counter++;
}

int asgard_get_device_count(void)
{
	return device_counter;
}
EXPORT_SYMBOL(asgard_get_device_count);