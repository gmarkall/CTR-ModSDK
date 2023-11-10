#include <common.h>

void DECOMP_MM_TrackSelect_Video_Draw(RECT *r, struct MainMenu_LevelRow *selectMenu, int trackIndex, int param_4, u_short param_5)
{
  u_char u0;
  u_char v0;
  u_short tpage;
  struct GameTracker* gGT = sdata->gGT;
  struct BigHeader* bh = sdata->ptrBigfileCdPos_2;
  struct BigEntry* entry = BIG_GETENTRY(bh);
  int videoID;

  selectMenu = &selectMenu[trackIndex];
  videoID = selectMenu->videoID;

  if (
		(entry[videoID].size == 0) || 
		
		// Video off-screen
		(r->x < 0) || 
		(r->y < 0) ||
		((r->x + r->w) > 0x200) || 
		((r->y + r->h) > 0xd8) 
    )
  {
    // draw icon
    OVR_230.trackSel_video_state = 1;
  }

  else
  {
    // Lock OVR_230.trackSel_video_state to zero to prevent allocation,
    // which helps the Oxide efforts

    if ((OVR_230.trackSel_video_state == 2) && (OVR_230.trackSel_unk == 1))
    {

      // If you have not allocated memory for video yet
      if (OVR_230.trackSel_video_boolAllocated == 0)
      {
        // Allocate memory for video in Track Selection
        MM_Video_AllocMem(0xb0, 0x4b, 4, 0, 0);

        // You have now allocated the memory
        OVR_230.trackSel_video_boolAllocated = OVR_230.trackSel_unk;
      }

      // CD position of video, and numFrames
      MM_Video_StartStream(bh->cdpos + entry[videoID].offset, selectMenu->videoLength);
    }
	
    if (((OVR_230.trackSel_unk == 3) || (OVR_230.trackSel_video_state == 3)) || (OVR_230.trackSel_video_state == 2))
    {
      tpage = gGT->ptrIcons[0x3f]->texLayout.tpage;
      u0 = gGT->ptrIcons[0x3f]->texLayout.u0;
      v0 = gGT->ptrIcons[0x3f]->texLayout.v0;

	  int ret = 
		MM_Video_DecodeFrame(
			gGT->db[1-gGT->swapchainIndex].drawEnv.ofs[0] + (r->x + 3),
			gGT->db[1-gGT->swapchainIndex].drawEnv.ofs[1] + (r->y + 2)	  
		);

      if ((ret == 1) && (OVR_230.trackSel_video_state == 2))
      {
        OVR_230.trackSel_video_state = 3;
      }
      if (OVR_230.trackSel_unk == 3)
      {
        // RECT position (x,y)
        sdata->videoSTR_src_vramRect.x = (u_short)u0 + (tpage & 0xf) * 0x40 + 3;
        sdata->videoSTR_src_vramRect.y = (u_short)v0 + (tpage & 0x10) * 0x10 + (short)(((u_int)tpage & 0x800) >> 2) + 2;

        // RECT size (w,h)
        sdata->videoSTR_src_vramRect.w = 0xaa;
        sdata->videoSTR_src_vramRect.h = 0x47;

        // VRAM destination (x,y) on swapchain image
        sdata->videoSTR_dst_vramX = gGT->db[gGT->swapchainIndex].dispEnv.disp.x + (r->x + 3);
        sdata->videoSTR_dst_vramY = gGT->db[gGT->swapchainIndex].dispEnv.disp.y + (r->y + 2);

        // enable video copy, give src and dst
        MainFrame_InitVideoSTR(1, &sdata->videoSTR_src_vramRect.x, sdata->videoSTR_dst_vramX, sdata->videoSTR_dst_vramY);
      }
    }
  }

  // if not playing video, draw icon
  if (OVR_230.trackSel_video_state != 3)
  {
    // This is the same function that draws Character icons

    // Draw Video icon
    MENUBOX_DrawPolyGT4(gGT->ptrIcons[selectMenu->videoThumbnail],
                        (r->x + 3), (r->y + 2),

                        // pointer to PrimMem struct
                        &gGT->backBuffer->primMem,

                        // pointer to OT mem
                        gGT->tileView_UI.ptrOT,

                        *(int*)0x800b55c0,
						*(int*)0x800b55c0,
						*(int*)0x800b55c0,
						*(int*)0x800b55c0,
						0, FP(1.0));
  }

  if (OVR_230.trackSel_unk == 1)
  {
    // disable video copy
    MainFrame_InitVideoSTR(0, 0, 0, 0);
  }
  if ((param_4 == 1) && (OVR_230.trackSel_video_boolAllocated == 1))
  {
    OVR_230.trackSel_video_state = 1;
  }
  if ((OVR_230.trackSel_video_state == 1) && (OVR_230.trackSel_unk != 1))
  {
    MM_Video_StopStream();
  }
  if ((param_4 == 1) && (OVR_230.trackSel_video_boolAllocated == 1))
  {
    MM_Video_ClearMem(r, (u_int)param_5 | 1);

    OVR_230.trackSel_video_boolAllocated = 0;
  }
  OVR_230.trackSel_unk = OVR_230.trackSel_video_state;

  // Draw 2D Menu rectangle background
  MENUBOX_DrawInnerRect(r, (short)(param_5 | 1), gGT->backBuffer->otMem.startPlusFour);
}