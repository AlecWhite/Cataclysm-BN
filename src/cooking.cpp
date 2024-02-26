#include "player.h"
#include "cooking.h"
#include "messages.h"
#include "ui_manager.h"
#include <input.h>
#include "output.h"
using namespace catacurses;

bool isWide = false;
int windowHeight = 5;
int windowWidth;

void cooking_ui() {
    // Needed for UI to draw
    ui_adaptor ui;

    // Ncurses window for drawing
    catacurses::window windowHead;
    // Takes care of the resizing procress
    ui.on_screen_resize([&](ui_adaptor& ui) {
    
    //Cargo culting from crafting_gui to the the correct width
    const int freeWidth = TERMX - FULL_SCREEN_WIDTH;
    isWide = (TERMX > FULL_SCREEN_WIDTH && freeWidth > 15);
    windowWidth = isWide ? (freeWidth > FULL_SCREEN_WIDTH ? FULL_SCREEN_WIDTH * 2 : TERMX) :
        FULL_SCREEN_WIDTH;
    int wStart = (TERMX - windowWidth) / 2;
    // Create an ncurses window
    
    windowHead = catacurses::newwin(windowHeight,windowWidth, point(wStart,0));



    // The window passed to this call must contain all the space the redraw
    // callback draws to, to ensure proper refreshing when resizing or exiting
    // from other UIs.
    ui.position_from_window(windowHead);
         });
    // Mark the resize callback to be called on the first redraw
    ui.mark_resize();
    // Things to do when redrawing the UI
    ui.on_redraw([&](const ui_adaptor&) {
    // Clear UI area
    werase(windowHead);

    // Print things
    mvwprintw(windowHead, point_zero, "Cooking menu, work in progress, ask again in two to three months");

    // Write to frame buffer
    wnoutrefresh(windowHead);
    });
    add_msg("cooking_ui text");
    
    // Vodoo stuff to avoid the window for instat closing
    input_context ctxt("<CATEGORY_NAME>");
    ctxt.register_action("QUIT");
    while (true) {
        add_msg("context loop");
        // Invalidate the top UI (that is, this UI) and redraw all
        // invalidated UIs (including lower UIs that calls this UI).
        // May call the resize callbacks.
        ui_manager::redraw();
        // Get user input. Note that this may call the resize and redraw
        // callbacks multiple times due to screen resize, rendering target
        // reset, etc.
        const std::string action = ctxt.handle_input();
        if (action == "QUIT") {
            break;
            
        }
        
    }
}

void player::cook() {
    cooking_ui();
    add_msg("<insert cook window>");
}
