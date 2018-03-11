library verilog;
use verilog.vl_types.all;
entity cal_flags32 is
    port(
        op              : in     vl_logic_vector(2 downto 0);
        result          : in     vl_logic_vector(31 downto 0);
        co_add          : in     vl_logic;
        co_prev_add     : in     vl_logic;
        co_sub          : in     vl_logic;
        co_prev_sub     : in     vl_logic;
        c               : out    vl_logic;
        n               : out    vl_logic;
        z               : out    vl_logic;
        v               : out    vl_logic
    );
end cal_flags32;
