#ifndef _ASGARD_CON_H
#define _ASGARD_CON_H

#define ASGARD_MLX5_DEVICES_LIMIT 5 /* Number of allowed mlx5 devices that can connect to ASGARD */


struct asgard_mlx5_con_info {
	int ix[3];
	u32 cqn[3];
	void *c[3]; /* mlx5 channel */

	int ifindex;

	int asgard_alive;

	int ctype;

	void (*asgard_post_ts)(int, uint64_t, int);
	void (*asgard_post_payload)(int, void *, u16, u32);
};

int asgard_generate_next_id(void);
void asgard_mlx5_post_optimistical_timestamp(int asgard_id, uint64_t cycle_ts, int ctype);
bool asgard_mlx5_post_payload(int asgard_id, void *va, u32 frag_size, u16 headroom,
			    u32 cqe_bcnt, int ctype);

int asgard_mlx5_con_check_ix(int asgard_id, int ix);
int asgard_mlx5_con_check_cqn(int asgard_id, u32 cqn);
void *asgard_mlx5_get_channel(int asgard_id);
int asgard_mlx5_con_register_channel(int asgard_id, int ix, u32 cqn, void *c, int ctype);
int asgard_mlx5_con_register_device(int ifindex);

int register_asgard_at_nic(int asgard_id,
				void (*asgard_post_ts)(int, uint64_t, int),
				void (*asgard_post_payload)(int, void *, u16, u32));
				
void unregister_asgard(void);

int get_asgard_id_by_ifindex(int ifindex);

int asgard_get_device_count(void);

#endif  /* _ASGARD_CON_H */