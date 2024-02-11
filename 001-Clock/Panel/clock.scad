include <../../AEPanelMaker/AEPanelMaker/panel.scad>

title = "clock";
title_x = 6;
title_y = 88 ;
title_font_size = 5;
title_font = "Liberation Sans:style=bold";
title_rotate = 90;

units = 1;
inputs = 0;
outputs = 8;

pots_rv09 = [
    [3, 13, "Tempo"],
    [3, 3, "Duty"]
];

leds = [
    [3, 25],
];

generate_panel();