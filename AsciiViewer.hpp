//May 14
//
#ifndef ASCII_VIEWER_HPP_
#define ASCII_VIEWER_HPP_

#include <iostream>
#include "Lattice.hpp"


#ifdef __APPLE__
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <memory> //for shared_pt

//Forwards lattice to cout
//fulfills concepts required for VisualizationPolicy in Simulation.hpp
namespace Sim {


class AsciiViewer {
public:

    AsciiViewer() {
        initscr();
        start_color();
        init_pair(1,COLOR_BLUE,COLOR_WHITE);
        color_set(1,0);
        curs_set(0); /* Makes the cursor invisible */

    }

    template <typename SpinType>
    void update_view(const Sim::Lattice<SpinType>& lattice) const {
        for (int row = lattice.HEIGHT -1; row >= 0; --row) {
            for (int col = 0; col < lattice.WIDTH; ++col) {
                SpinType s = lattice(row,col);
                //assert(s >= 0 && s <= 9 && "spin can't be negative, otherwise visualization fails");
                //or simply map all negative values to 0...
                if (s < 0) {
                    s = 0;
                }
                mvprintw(row,col,"%i",s);
            }
            chtype s = '\n';
            mvaddch(row,lattice.WIDTH,s);
        }
        refresh();
        getch();
    }

    ~AsciiViewer() {
        endwin();
    }


};




}



#endif
