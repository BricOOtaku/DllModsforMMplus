#include <Windows.h>
#include <vector>
#include "SigScan.h"

//Hook
SIG_SCAN(sigStartSong, 0x14040B600, "\x41\x54\x41\x55\x41\x57\x48\x83\xEC\x30\x4C\x8B\xFA\x45\x0F", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigModifiers, 0x14027BEE0, "\x48\x8B\x01\x89\x50\x20\xC3\xCC", "xxxxxxxx")
SIG_SCAN(sigPv, 0x14043B310, "\x8B\xD1\xE9\xA9\xE8\xFF\xFF\xCC", "xxxxxxxx")


//Hit Effects
SIG_SCAN(sigHitEff00, 0x140C88450, "hit_eff00", "xxxxxxxxx")
SIG_SCAN(sigHitEff01, 0x140C8A278, "hit_eff01", "xxxxxxxxx")
SIG_SCAN(sigHitEff02, 0x140C8A288, "hit_eff02", "xxxxxxxxx")
SIG_SCAN(sigHitEff03, 0x140C8A258, "hit_eff03", "xxxxxxxxx")
SIG_SCAN(sigHitEff04, 0x140C8A2A8, "hit_eff04", "xxxxxxxxx")

SIG_SCAN(sigHitEff00L, 0x140C8A268, "hit_eff00_l", "xxxxxxxxxxx")
SIG_SCAN(sigHitEff00R, 0x140C8A370, "hit_eff00_r", "xxxxxxxxxxx")
SIG_SCAN(sigHitEff01L, 0x140C8A360, "hit_eff01_l", "xxxxxxxxxxx")
SIG_SCAN(sigHitEff01R, 0x140C8A338, "hit_eff01_r", "xxxxxxxxxxx")

SIG_SCAN(sigHitEffSlide01L, 0x140C8A348, "hit_eff_slide01_l", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigHitEffSlide01R, 0x140C8A308, "hit_eff_slide01_r", "xxxxxxxxxxxxxxxxx")


//Bottom
SIG_SCAN(sigSongEnergyClearTxt, 0x140C8ACB0, "song_energy_clear_txt", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigPrcGaugeBaseArcade, 0x140CC0F40, "prc_gauge_base_arcade", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigSongEnergyReach, 0x140C8AD10, "song_energy_reach", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyBorder, 0x140C8ADA0, "song_energy_border\000", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyBase, 0x140C8AD28, "song_energy_base", "xxxxxxxxxxxxxxxx")


//Judgments Values
SIG_SCAN(sigComboTempoFine001, 0x140C8AC50, "combo_tempo_fine001\000", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine010, 0x140C8AC08, "combo_tempo_fine010\000", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine100, 0x140C8AC20, "combo_tempo_fine100\000", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine100B, 0x140C8ABD8, "combo_tempo_fine100_b\000", "xxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoFine1000, 0x140C8ABF0, "combo_tempo_fine1000\000", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigComboFine001, 0x140C8AAC8, "combo_fine001\000", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine010, 0x140C8AAD8, "combo_fine010\000", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine100, 0x140C8AAA8, "combo_fine100\000", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine100B, 0x140C8AAB8, "combo_fine100_b\000", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboFine1000, 0x140C8ABB8, "combo_fine1000\000", "xxxxxxxxxxxxxxx")

SIG_SCAN(sigValueTextFine01, 0x140C8A978, "value_text_fine01", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextFine03, 0x140C8A9F0, "value_text_fine03", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigMixModeValueTextFine, 0x140CBF108, "value_text_fine", "xxxxxxxxxxxxxxx")

SIG_SCAN(sigValueTextSad, 0x140C8AA98, "value_text_sad", "xxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextWorst, 0x140C8AA68, "value_text_worst", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigOptionInfoHiddenA, 0x140C8B188, "option_info_hidden_a", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigOptionInfoHispeedA, 0x140C8B1B8, "option_info_hispeed_a", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigOptionInfoSuddenA, 0x140C8B158, "option_info_sudden_a", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigOptionInfoBase, 0x140C8B1A0, "option_info_base", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigSongEnergyBorderExcellent, 0x140C8AE10, "song_energy_border_excellent", "xxxxxxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyBorderGreat, 0x140C8ADF0, "song_energy_border_great", "xxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyEffReach, 0x140C8AC98, "song_energy_eff_reach", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigSongEnergyEdgeLine, 0x140CBE528, "song_energy_edge_line", "xxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigValueTextCool01, 0x140C8AA20, "value_text_cool01", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigValueTextCool03, 0x140C8AA38, "value_text_cool03", "xxxxxxxxxxxxxxxxx")

SIG_SCAN(sigComboTempoCool001, 0x140C8AB48, "combo_tempo_cool001\000", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool010, 0x140C8AB60, "combo_tempo_cool010\000", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool100, 0x140C8AC68, "combo_tempo_cool100\000", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool100B, 0x140C8AC80, "combo_tempo_cool100_b\000", "xxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboTempoCool1000, 0x140C8AC38, "combo_tempo_cool1000\000", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigComboCool001, 0x140C8ABC8, "combo_cool001\000", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool010, 0x140C8AB98, "combo_cool010\000", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool100, 0x140C8ABA8, "combo_cool100\000", "xxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool100B, 0x140C8AB78, "combo_cool100_b\000", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigComboCool1000, 0x140C8AB88, "combo_cool1000\000", "xxxxxxxxxxxxxxx")

SIG_SCAN(sigPEnergyNum01C, 0x140C8B3D8, "p_energy_num01_c", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum02C, 0x140C8B4D0, "p_energy_num02_c", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum03C, 0x140C8B4E8, "p_energy_num03_c", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum04C, 0x140C8B4A0, "p_energy_num04_c", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum05C, 0x140C8B4B8, "p_energy_num05_c", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum06C, 0x140C8B470, "p_energy_num06_c", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigPEnergyNum07C, 0x140C8B488, "p_energy_num07_c", "xxxxxxxxxxxxxxxx")


//Failed and Patch for FT / MM UI compatibility
SIG_SCAN(sigFailedInfo, 0x140C8A828, "failed_info", "xxxxxxxxxxx")

SIG_SCAN(sigPauseWin01, 0x140CC2378, "pause_win_01", "xxxxxxxxxxxx")
SIG_SCAN(sigFadeBlack, 0x140C822E0, "fade_black", "xxxxxxxxxx")
SIG_SCAN(sigSubBaseBg01, 0x140C82220, "sub_base_bg_01", "xxxxxxxxxxxxxx")

SIG_SCAN(sigPrcInfo, 0x140C8B668, "prc_info", "xxxxxxxx")
SIG_SCAN(sigPrcBg, 0x140CBEB70, "prc_bg", "xxxxxx")
SIG_SCAN(sigSuspendArcade, 0x140C8B6A8, "suspend_arcade", "xxxxxxxxxxxxxx")

SIG_SCAN(sigKeyHelpAnm, 0x140CC1030, "keyhelp_anm", "xxxxxxxxxxx")
SIG_SCAN(sigPvInfoSong, 0x140C83AA0, "pv_info_song", "xxxxxxxxxxxx")


//Modifiers
SIG_SCAN(sigPvIconRep03, 0x140C839D0, "pv_icon_rep03", "xxxxxxxxxxxxx")
SIG_SCAN(sigPvIconRep02, 0x140C839F8, "pv_icon_rep02", "xxxxxxxxxxxxx")
SIG_SCAN(sigPvIconRep01, 0x140C83A48, "pv_icon_rep01", "xxxxxxxxxxxxx")
SIG_SCAN(sigPvIconShuf02, 0x140C83A20, "pv_icon_shuf02", "xxxxxxxxxxxxxx")
SIG_SCAN(sigPvIconShuf01, 0x140C83970, "pv_icon_shuf01", "xxxxxxxxxxxxxx")


// Playstation/Arcade
SIG_SCAN(sigTargetMaru, 0x140C88430, "target_maru", "xxxxxxxxxxx")
SIG_SCAN(sigTargetBatsu, 0x140C89A50, "target_batsu", "xxxxxxxxxxxx")
SIG_SCAN(sigTargetShikaku, 0x140C89A60, "target_shikaku", "xxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSankaku, 0x140C89A88, "target_sankaku", "xxxxxxxxxxxxxx")

SIG_SCAN(sigTargetMaruSync, 0x140C89E28, "target_maru_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetBatsuSync, 0x140C89DE0, "target_batsu_sync", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetShikakuSync, 0x140C89DF8, "target_shikaku_sync", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSankakuSync, 0x140C89E10, "target_sankaku_sync", "xxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetMaruHold, 0x140C89BA0, "target_maru_hold", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetBatsuHold, 0x140C89B58, "target_batsu_hold", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetShikakuHold, 0x140C89B70, "target_shikaku_hold", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSankakuHold, 0x140C89B88, "target_sankaku_hold", "xxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetMaruSynchold, 0x140C89DC8, "target_maru_synchold", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetBatsuSynchold, 0x140C89D80, "target_batsu_synchold", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetShikakuSynchold, 0x140C89D98, "target_shikaku_synchold", "xxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSankakuSynchold, 0x140C89DB0, "target_sankaku_synchold", "xxxxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSpMaru, 0x140C8A540, "target_sp_maru", "xxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpBatsu, 0x140C8A550, "target_sp_batsu", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpShikaku, 0x140C8A510, "target_sp_shikaku", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpSankaku, 0x140C8A578, "target_sp_sankaku", "xxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSpMaruSync, 0x140C8A5D8, "target_sp_maru_sync", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpBatsuSync, 0x140C8A590, "target_sp_batsu_sync", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpShikakuSync, 0x140C8A5A8, "target_sp_shikaku_sync", "xxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpSankakuSync, 0x140C8A5C0, "target_sp_sankaku_sync", "xxxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigButtonMaru, 0x140C88440, "button_maru", "xxxxxxxxxxx")
SIG_SCAN(sigButtonBatsu, 0x140C89CB0, "button_batsu", "xxxxxxxxxxxx")
SIG_SCAN(sigButtonShikaku, 0x140C89CC0, "button_shikaku", "xxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSankaku, 0x140C89CE8, "button_sankaku", "xxxxxxxxxxxxxx")

SIG_SCAN(sigButtonMaruSync, 0x140C89ED0, "button_maru_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonBatsuSync, 0x140C89F00, "button_batsu_sync", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonShikakuSync, 0x140C89F18, "button_shikaku_sync", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSankakuSync, 0x140C89F48, "button_sankaku_sync", "xxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigShadowMaru, 0x140C8A0A8, "shadow_maru", "xxxxxxxxxxx")
SIG_SCAN(sigShadowBatsu, 0x140C8A0B8, "shadow_batsu", "xxxxxxxxxxxx")
SIG_SCAN(sigShadowShikaku, 0x140C8A080, "shadow_shikaku", "xxxxxxxxxxxxxx")
SIG_SCAN(sigShadowSankaku, 0x140C8A0E0, "shadow_sankaku", "xxxxxxxxxxxxxx")

SIG_SCAN(sigWhiteMaru, 0x140C8A238, "white_maru", "xxxxxxxxxx")
SIG_SCAN(sigWhiteBatsu, 0x140C8A248, "white_batsu", "xxxxxxxxxxx")
SIG_SCAN(sigWhiteShikaku, 0x140C8A210, "white_shikaku", "xxxxxxxxxxxxx")
SIG_SCAN(sigWhiteSankaku, 0x140C8A130, "white_sankaku", "xxxxxxxxxxxxx")


// Switch/Xbox Buttons
SIG_SCAN(sigButtonYbtn, 0x140C89D18, "button_ybtn", "xxxxxxxxxxx")
SIG_SCAN(sigButtonAbtn, 0x140C89D38, "button_abtn", "xxxxxxxxxxx")
SIG_SCAN(sigButtonBbtn, 0x140C89D48, "button_bbtn", "xxxxxxxxxxx")
SIG_SCAN(sigButtonXbtn, 0x140C89D70, "button_xbtn", "xxxxxxxxxxx")

SIG_SCAN(sigButton2Ybtn, 0x140C89D28, "button2_ybtn", "xxxxxxxxxxxx")
SIG_SCAN(sigButton2Abtn, 0x140C89D08, "button2_abtn", "xxxxxxxxxxxx")
SIG_SCAN(sigButton2Bbtn, 0x140C89CF8, "button2_bbtn", "xxxxxxxxxxxx")

SIG_SCAN(sigButtonYbtnSync, 0x140C89FD8, "button_ybtn_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonAbtnSync, 0x140C8A008, "button_abtn_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonBbtnSync, 0x140C89FC0, "button_bbtn_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonXbtnSync, 0x140C89FF0, "button_xbtn_sync", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigButton2YbtnSync, 0x140C8A0F0, "button2_ybtn_sync", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigButton2AbtnSync, 0x140C8A0C8, "button2_abtn_sync", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigButton2BbtnSync, 0x140C8A108, "button2_bbtn_sync", "xxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetYbtn, 0x140C89BE8, "target_ybtn", "xxxxxxxxxxx")
SIG_SCAN(sigTargetAbtn, 0x140C89C10, "target_abtn", "xxxxxxxxxxx")
SIG_SCAN(sigTargetBbtn, 0x140C89C20, "target_bbtn", "xxxxxxxxxxx")
SIG_SCAN(sigTargetXbtn, 0x140C89C48, "target_xbtn", "xxxxxxxxxxx")

SIG_SCAN(sigTargetYbtnSync, 0x140C89F90, "target_ybtn_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetAbtnSync, 0x140C89E40, "target_abtn_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetBbtnSync, 0x140C89E58, "target_bbtn_sync", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetXbtnSync, 0x140C89E88, "target_xbtn_sync", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetYbtnHold, 0x140C89CD0, "target_ybtn_hold", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetAbtnHold, 0x140C89BB8, "target_abtn_hold", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetBbtnHold, 0x140C89BD0, "target_bbtn_hold", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetXbtnHold, 0x140C89BF8, "target_xbtn_hold", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetYbtnSynchold, 0x140C89F30, "target_ybtn_synchold", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetAbtnSynchold, 0x140C89F60, "target_abtn_synchold", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetBbtnSynchold, 0x140C89F78, "target_bbtn_synchold", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetXbtnSynchold, 0x140C89FA8, "target_xbtn_synchold", "xxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSpYbtn, 0x140C8A600, "target_sp_ybtn", "xxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpAbtn, 0x140C8A620, "target_sp_abtn", "xxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpBbtn, 0x140C8A5F0, "target_sp_bbtn", "xxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpXbtn, 0x140C8A610, "target_sp_xbtn", "xxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSpYbtnSync, 0x140C8A678, "target_sp_ybtn_sync", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpAbtnSync, 0x140C8A6A8, "target_sp_abtn_sync", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpBbtnSync, 0x140C8A660, "target_sp_bbtn_sync", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpXbtnSync, 0x140C8A690, "target_sp_xbtn_sync", "xxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigShadowYbtn, 0x140C8A120, "shadow_ybtn", "xxxxxxxxxxx")
SIG_SCAN(sigShadowAbtn, 0x140C8A140, "shadow_abtn", "xxxxxxxxxxx")
SIG_SCAN(sigShadowBbtn, 0x140C8A150, "shadow_bbtn", "xxxxxxxxxxx")
SIG_SCAN(sigShadowXbtn, 0x140C8A178, "shadow_xbtn", "xxxxxxxxxxx")

SIG_SCAN(sigWhiteYbtnSync, 0x140C8A298, "white_ybtn_sync", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigWhiteAbtnSync, 0x140C8A2B8, "white_abtn_sync", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigWhiteBbtnSync, 0x140C8A2C8, "white_bbtn_sync", "xxxxxxxxxxxxxxx")
SIG_SCAN(sigWhiteXbtnSync, 0x140C8A1D0, "white_xbtn_sync", "xxxxxxxxxxxxxxx")


// Sliders
SIG_SCAN(sigButtonSlide01, 0x140C89C88, "button_slide01", "xxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSlide18L, 0x140C89C98, "button_slide18_l", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSlide18R, 0x140C89C58, "button_slide18_r", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSlide25L, 0x140C89C70, "button_slide25_l", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSlide25R, 0x140C89D58, "button_slide25_r", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigButtonSlide18LSync, 0x140C8A050, "button_slide18_l_sync", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSlide18RSync, 0x140C8A068, "button_slide18_r_sync", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSlide25LSync, 0x140C8A020, "button_slide25_l_sync", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigButtonSlide25RSync, 0x140C8A038, "button_slide25_r_sync", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSlide01Notxt, 0x140C89B28, "target_slide01_notxt", "xxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlide18L, 0x140C89B40, "target_slide18_l", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlide18R, 0x140C89AF8, "target_slide18_r", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlide26L, 0x140C89EB8, "target_slide26_l", "xxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlide26R, 0x140C89C30, "target_slide26_r", "xxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSlideB18L, 0x140C8A738, "target_slide18b_l", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlideB18R, 0x140C8A750, "target_slide18b_r", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlideB26L, 0x140C8A630, "target_slide26b_l", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlideB26R, 0x140C8A648, "target_slide26b_r", "xxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSlide18LSync, 0x140C89EE8, "target_slide18_l_sync", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlide18RSync, 0x140C89EA0, "target_slide18_r_sync", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlide26LSync, 0x140C89EB8, "target_slide26_l_sync", "xxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSlide26RSync, 0x140C89E70, "target_slide26_r_sync", "xxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigTargetSpSlide01Notxt, 0x140C8A528, "target_sp_slide01_notxt", "xxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpSlide18L, 0x140C8A4E0, "target_sp_slide18_l", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpSlide18R, 0x140C8A4F8, "target_sp_slide18_r", "xxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpSlide18LSync, 0x140C8A6C0, "target_sp_slide18_l_sync", "xxxxxxxxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigTargetSpSlide18RSync, 0x140C8A6E0, "target_sp_slide18_r_sync", "xxxxxxxxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigShadowSlideL18L, 0x140C8A090, "shadow_slide_l18_l", "xxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigShadowSlideL18R, 0x140C8A188, "shadow_slide_l18_r", "xxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigShadowSlideS25L, 0x140C8A1A0, "shadow_slide_s25_l", "xxxxxxxxxxxxxxxxxx")
SIG_SCAN(sigShadowSlideS25R, 0x140C8A160, "shadow_slide_s25_r", "xxxxxxxxxxxxxxxxxx")

SIG_SCAN(sigWhiteSlideL18L, 0x140C8A220, "white_slide_l18_l", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigWhiteSlideL18R, 0x140C8A1E0, "white_slide_l18_r", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigWhiteSlideS25L, 0x140C8A1F8, "white_slide_s25_l", "xxxxxxxxxxxxxxxxx")
SIG_SCAN(sigWhiteSlideS25R, 0x140C8A1B8, "white_slide_s25_r", "xxxxxxxxxxxxxxxxx")


// Arrays
std::vector<void*> FTPatchXmmArrays = {
	(char*)sigPauseWin01() + 0x06,
	sigFadeBlack(),
	sigPrcInfo(),
	sigKeyHelpAnm(),
};

std::vector<void*> FTPatchMmArrays = {
	//(char*)sigSubBaseBg01() + 0x09, /!\Cause softlock on Custom Playlist
	(char*)sigPrcBg() + 0x04,
	sigSuspendArcade(),
	sigPvIconRep03(),
	sigPvIconRep02(),
	sigPvIconRep01(),
	sigPvIconShuf02(),
	sigPvIconShuf01(),
	sigPvInfoSong(),
};

std::vector<void*> HitEffLArrays = {
	sigHitEff00L(),
	sigHitEff01L()
};

std::vector<void*> HitEffRArrays = {
	sigHitEff00R(),
	sigHitEff01R()
};

std::vector<void*> HitEffArrays = {
	sigHitEff00(),
	sigHitEff01(),
	sigHitEff02(),
	sigHitEff03(),
	sigHitEff04(),
	sigHitEffSlide01L(),
	sigHitEffSlide01R()
};

std::vector<void*> songEnergyAArrays = {
	sigSongEnergyReach(),
	sigSongEnergyBorder(),
	sigSongEnergyBase()
};

std::vector<void*> comboFineArrays = {
	sigComboTempoFine001(), sigComboTempoFine010(), sigComboTempoFine100(), sigComboTempoFine100B(), sigComboTempoFine1000(),
	sigComboFine001(), sigComboFine010(), sigComboFine100(), sigComboFine100B(), sigComboFine1000()
};

std::vector<void*> valueFineArrays = {
	sigValueTextFine01(),
	sigValueTextFine03()
};

std::vector<void*> targetArrays = {
	sigTargetMaru(), sigTargetBatsu(), sigTargetShikaku(), sigTargetSankaku(),
	sigTargetMaruSync(), sigTargetBatsuSync(), sigTargetShikakuSync(), sigTargetSankakuSync(),
	sigTargetMaruHold(), sigTargetBatsuHold(), sigTargetShikakuHold(),  sigTargetSankakuHold(),
	sigTargetMaruSynchold(),  sigTargetBatsuSynchold(),  sigTargetShikakuSynchold(), sigTargetSankakuSynchold(),
	sigTargetSpMaru(), sigTargetSpBatsu(), sigTargetSpShikaku(), sigTargetSpSankaku(),
	sigTargetSpMaruSync(), sigTargetSpBatsuSync(), sigTargetSpShikakuSync(), sigTargetSpSankakuSync()
};

std::vector<void*> buttonArrays = {
		sigButtonMaru(), sigButtonBatsu(), sigButtonShikaku(), sigButtonSankaku(),
		sigButtonMaruSync(), sigButtonBatsuSync(), sigButtonShikakuSync(), sigButtonSankakuSync(),
		sigShadowMaru(), sigShadowBatsu(), sigShadowShikaku(), sigShadowSankaku(),
		sigWhiteMaru(), sigWhiteBatsu(), sigWhiteShikaku(), sigWhiteSankaku(),

		sigButtonYbtn(), sigButtonAbtn(), sigButtonBbtn(), sigButtonXbtn(),
		sigButton2Ybtn(), sigButton2Abtn(), sigButton2Bbtn(),
		sigButtonYbtnSync(), sigButtonAbtnSync(), sigButtonBbtnSync(), sigButtonXbtnSync(),
		sigButton2YbtnSync(), sigButton2AbtnSync(), sigButton2BbtnSync(),
		sigTargetYbtn(), sigTargetAbtn(), sigTargetBbtn(), sigTargetXbtn(),
		sigTargetYbtnSync(), sigTargetAbtnSync(), sigTargetBbtnSync(), sigTargetXbtnSync(),
		sigTargetYbtnHold(), sigTargetAbtnHold(), sigTargetBbtnHold(), sigTargetXbtnHold(),
		sigTargetYbtnSynchold(), sigTargetAbtnSynchold(), sigTargetBbtnSynchold(), sigTargetXbtnSynchold(),
		sigTargetSpYbtn(), sigTargetSpAbtn(), sigTargetSpBbtn(), sigTargetSpXbtn(),
		sigTargetSpYbtnSync(), sigTargetSpAbtnSync(), sigTargetSpBbtnSync(), sigTargetSpXbtnSync(),
		sigShadowYbtn(), sigShadowAbtn(), sigShadowBbtn(), sigShadowXbtn(),
		sigWhiteYbtnSync(), sigWhiteAbtnSync(), sigWhiteBbtnSync(), sigWhiteXbtnSync(),

		sigButtonSlide01(), sigButtonSlide18L(), sigButtonSlide18R(), sigButtonSlide25L(), sigButtonSlide25R(),
		sigButtonSlide18LSync(), sigButtonSlide18RSync(), sigButtonSlide25LSync(), sigButtonSlide25RSync(),
		sigTargetSlide01Notxt(), sigTargetSlide18L(), sigTargetSlide18R(), sigTargetSlide26L(), sigTargetSlide26R(),
		sigTargetSlideB18L(), sigTargetSlideB18R(), sigTargetSlideB26L(), sigTargetSlideB26R(),
		sigTargetSlide18LSync(), sigTargetSlide18RSync(), sigTargetSlide26LSync(), sigTargetSlide26RSync(),
		sigTargetSpSlide01Notxt(), sigTargetSpSlide18L(), sigTargetSpSlide18R(), sigTargetSpSlide18LSync(), sigTargetSpSlide18RSync(),
		sigShadowSlideL18L(), sigShadowSlideL18R(), sigShadowSlideS25L(), sigShadowSlideS25R(),
		sigWhiteSlideL18L(), sigWhiteSlideL18R(), sigWhiteSlideS25L(), sigWhiteSlideS25R()
};

std::vector<void*> optionInfoArrays = {
	sigOptionInfoHiddenA(),
	sigOptionInfoHispeedA(),
	sigOptionInfoSuddenA(),
	sigOptionInfoBase()
};

std::vector<void*> songEnergyBArrays = {
	sigSongEnergyBorderExcellent(),
	sigSongEnergyBorderGreat(),
	sigSongEnergyEffReach(),
	sigSongEnergyEdgeLine()
};

std::vector<void*> valueTextArrays = {
	sigValueTextSad(),
	sigMixModeValueTextFine(),
	sigValueTextWorst()
};

std::vector<void*> coolArrays = {
	sigValueTextCool01(),
	sigValueTextCool03(),
	sigComboTempoCool001(),
	sigComboTempoCool010(),
	sigComboTempoCool100(),
	sigComboTempoCool100B(),
	sigComboTempoCool1000(),
	sigComboCool001(),
	sigComboCool010(),
	sigComboCool100(),
	sigComboCool100B(),
	sigComboCool1000()
};

std::vector<void*> energyNumArrays = {
	sigPEnergyNum01C(),
	sigPEnergyNum02C(),
	sigPEnergyNum03C(),
	sigPEnergyNum04C(),
	sigPEnergyNum05C(),
	sigPEnergyNum06C(),
	sigPEnergyNum07C()
};