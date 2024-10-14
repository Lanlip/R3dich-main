class misc
{
public:
	void misc_feat();

	bool bypasserror = true;
	bool noidlecam = false;
	bool showbalance = false;
	bool busyspin = true;

	bool esp_box = true;
	bool esp_line = false;
	bool esp_name = false;

	bool banner_notify = false;
};
inline misc g_misc;

using Player = std::int32_t;