library verilog;
use verilog.vl_types.all;
entity fa_v2 is
    port(
        a               : in     vl_logic;
        b               : in     vl_logic;
        ci              : in     vl_logic;
        s               : out    vl_logic
    );
end fa_v2;
