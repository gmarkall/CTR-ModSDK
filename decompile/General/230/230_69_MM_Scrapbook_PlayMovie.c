#include <common.h>

void MM_Scrapbook_PlayMovie(struct MenuBox *mb)
{
    short lev;
    int cdPos;
	int getButtonPress;
	DRAWENV* ptrDrawEnv;
    const CdlLOC cdLoc;
    struct GameTracker *gGT = gGT;
	int isOn = TitleFlag_IsFullyOnScreen();

    // book state (0,1,2,3,4)
    switch (OVR_230.scrapbookState)
    {

    // Init State,
    // alter checkered flag
    case 0:
        if (isOn == 1)
        {
            // checkered flag, begin transition off-screen
            TitleFlag_BeginTransition(2);
        }

        // go to Load State
        OVR_230.scrapbookState = 1;

        mb->state &= ~NEEDS_TO_CLOSE;

        Audio_SetState_Safe(1);

        break;

    // Load State,
    // find the TEST.STR file
    case 1:

        // if not fully off screen
        if (TitleFlag_IsFullyOffScreen() != 1)
        {
            // quit, dont start video yet
            return;
        }

        CDSYS_SetMode_StreamData();

        // \TEST.STR;1
        // if file was found
        if (CdSearchFile(&cdLoc, OVR_230.s_teststr1) != 0)
        {
            SpuSetCommonCDVolume(sdata->vol_Music << 7, sdata->vol_Music << 7);

            // Alloc memory to store Scrapbook
            MM_Video_AllocMem(0x200, 0xd0, 10, 0x40, 1);

            cdPos = CdPosToInt(&cdLoc);

            // scrapbook runs 15fps,
            // see bottom of Duckstation screen while running

            // scrapbook is 4.91 min, 4 mins + 54 sec,
            // (4424 total / 15fps / 60 sec per min) mins,
            // 0x1148 = 4424 = numFrames

            // CD position of video, and numFrames
            MM_Video_StartStream(cdPos, 0x1148);

            // start playing movie
            OVR_230.scrapbookState = 2;

            return;
        }

        goto GO_BACK;

    // Actually play the movie
    case 2:
	
        // infinite loop (cause this is scrapbook),
        // keep doing DecodeFrame and VSync until done
        while (
			
			// This bugs for the same reason as RenderFrame_Vsync,
			// register wont update caues modern GCC doesn't know
			// swapchainIndex changes in VsyncCallback
			
			// gGT->DB[nextFrame] (swapchain flips in VsyncCallback)
			ptrDrawEnv = &gGT->db[1 - gGT->swapchainIndex].drawEnv,
            
			MM_Video_DecodeFrame(
				ptrDrawEnv->ofs[0],
				ptrDrawEnv->ofs[1] + 4) == 0)
        {
            VSync(0);
        }
		
		// If you press Start, Cross, Circle, Triangle, or Square
		getButtonPress = (sdata->buttonTapPerPlayer[0] & 0x41070);

        if (
				// if movie is finished,
				// means scrapbook ended, no looping
				(MM_Video_CheckIfFinished(0) == 1) ||
				(getButtonPress != 0)
			)
        {
            if (getButtonPress != 0)
            {
                TitleFlag_SetFullyOnScreen();
            }

            // stop video
            OVR_230.scrapbookState = 3;
        }

        VSync(4);
        break;

    // return disc to normal,
    // return checkered flag to normal
    case 3:
        SpuSetCommonCDVolume(0, 0);

        MM_Video_StopStream();

        MM_Video_ClearMem();

        if (TitleFlag_IsFullyOffScreen() == 1)
        {
            // checkered flag, begin transition on-screen
            TitleFlag_BeginTransition(1);
        }
    GO_BACK:

        // return to gameplay
        OVR_230.scrapbookState = 4;
        break;

    // send player back to adv hub,
    // or back to main menu
    case 4:
        if (isOn == 1)
        {
            // change checkered flag back
            TitleFlag_SetDrawOrder(0);

			// if adventure mode
			lev = GEM_STONE_VALLEY;

            // If you're not in Adventure Mode
            if ((gGT->gameMode1 & ADVENTURE_MODE) == 0)
            {
                lev = MAIN_MENU_LEVEL;
				
                MM_JumpTo_Title_Returning();

                // return to main menu (adv, tt, arcade, vs, battle)
                sdata->mainMenuState = 0;
            }

            MainRaceTrack_RequestLoad(lev);
            MENUBOX_Hide(mb);
        }
        break;
    default:
        return;
    }
}