#include "emu.h"
#include "includes/gbusters.h"


/***************************************************************************

  Callbacks for the K052109

***************************************************************************/

K052109_CB_MEMBER(gbusters_state::tile_callback)
{
	/* (color & 0x02) is flip y handled internally by the 052109 */
	*code |= ((*color & 0x0d) << 8) | ((*color & 0x10) << 5) | (bank << 12);
	*color = m_layer_colorbase[layer] + ((*color & 0xe0) >> 5);
}

/***************************************************************************

  Callbacks for the K051960

***************************************************************************/

K051960_CB_MEMBER(gbusters_state::sprite_callback)
{
	*priority = (*color & 0x30) >> 4;
	*color = m_sprite_colorbase + (*color & 0x0f);
}


/***************************************************************************

    Start the video hardware emulation.

***************************************************************************/

void gbusters_state::video_start()
{
	m_layer_colorbase[0] = 48;
	m_layer_colorbase[1] = 0;
	m_layer_colorbase[2] = 16;
	m_sprite_colorbase = 32;
}


UINT32 gbusters_state::screen_update_gbusters(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	m_k052109->tilemap_update();

	/* sprite priority 3 = disable */
	if (m_priority)
	{
//      m_k051960->k051960_sprites_draw(bitmap, cliprect, screen.priority(), 1, 1);  /* are these used? */
		m_k052109->tilemap_draw(screen, bitmap, cliprect, 2, TILEMAP_DRAW_OPAQUE, 0);
		m_k051960->k051960_sprites_draw(bitmap, cliprect, screen.priority(), 2, 2);
		m_k052109->tilemap_draw(screen, bitmap, cliprect, 1, 0, 0);
		m_k051960->k051960_sprites_draw(bitmap, cliprect, screen.priority(), 0, 0);
		m_k052109->tilemap_draw(screen, bitmap, cliprect, 0, 0, 0);
	}
	else
	{
//      m_k051960->k051960_sprites_draw(bitmap, cliprect, screen.priority(), 1, 1);  /* are these used? */
		m_k052109->tilemap_draw(screen, bitmap, cliprect, 1, TILEMAP_DRAW_OPAQUE, 0);
		m_k051960->k051960_sprites_draw(bitmap, cliprect, screen.priority(), 2, 2);
		m_k052109->tilemap_draw(screen, bitmap, cliprect, 2, 0, 0);
		m_k051960->k051960_sprites_draw(bitmap, cliprect, screen.priority(), 0, 0);
		m_k052109->tilemap_draw(screen, bitmap, cliprect, 0, 0, 0);
	}
	return 0;
}
