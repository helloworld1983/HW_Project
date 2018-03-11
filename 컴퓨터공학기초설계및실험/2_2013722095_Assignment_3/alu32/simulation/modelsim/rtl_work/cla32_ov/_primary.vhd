library verilog;
use verilog.vl_types.all;
entity cla32_ov is
    port(
        a               : in     vl_logic_vector(31 downto 0);
        b               : in     vl_logic_vector(31 downto 0);
        ci              : in     vl_logic;
        s               : out    vl_logic_vector(31 downto 0);
        co_prev         : out    vl_logic;
        co              : out    vl_logic
    );
end cla32_ov;
