library verilog;
use verilog.vl_types.all;
entity cla4_ov is
    port(
        a               : in     vl_logic_vector(3 downto 0);
        b               : in     vl_logic_vector(3 downto 0);
        ci              : in     vl_logic;
        s               : out    vl_logic_vector(3 downto 0);
        c3              : out    vl_logic;
        co              : out    vl_logic
    );
end cla4_ov;
