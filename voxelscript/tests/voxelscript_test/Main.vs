import Crusher;
import Fuels;
import Ores;
import Entity;

class Tester {
    init();
}
implement Tester {
    init() {
        Coal f = new Coal();
        Entity e = f;
        IronOre io = new IronOre();
        Crusher c = new Crusher();

        c.fuel.give(f, 1);
        c.crushing.give(io, 1);
        int i = 0;
        while(i < 200) {
            print("<progress=", c.current_progress, ",energy_in_crusher=", c.energy_in_crusher, ">");
            c.iterate();
            i++;
        }
    }
}

Tester t = new Tester();

export {};
