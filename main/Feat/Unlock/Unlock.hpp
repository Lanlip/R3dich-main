#include "../utility/logger/logger.hpp"
#include "../source/hooks/hooks.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/joaat.hpp"
#include "../Fonts/fonts.hpp"
#include <array>
#include "../script_global/script_global.hpp"
#include "../utility/rage/joaat.hpp"

using Hash = std::uint32_t;

class unlock
{
public:
	void unlock_feat();

	void unlock_stuff();
	void unlock_bunker();
	void unlock_clothes();
	void unlock_weapon();
	void unlock_vehicle();
	void unlock_rims();
	void unlock_para();
	void unlock_tattoos();
	void unlock_hair();
	void unlock_firework();
	void unlock_armor();
	void unlock_snack();
	void unlock_statue();
	void unlock_office_money();

	void unlock_fastrun();
	void unlock_gender();
	void unlock_stat();
	void unlock_achieve();
	void unlock_lsc();
	void unlock_chrims();
	void unlock_badsport();
	void unlock_xmas();
	void unlock_freeapp();
	void unlock_gunvan();
	void unlock_halodlc();

	void unlock_removed_vehicle();

	void unlock_all();
};
inline unlock g_unlock;