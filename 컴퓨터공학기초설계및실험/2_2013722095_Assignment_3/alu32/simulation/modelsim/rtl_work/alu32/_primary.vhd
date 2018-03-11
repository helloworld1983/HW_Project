library verilog;
use verilog.vl_types.all;
entity alu32 is
    port(
        a               : in     vl_logic_vector(31 downto 0);
        b               : in     vl_logic_vector(31 downto 0);
        op              : in     vl_logic_vector(2 downto 0);
        result          : out    vl_logic_vector(31 downto 0);
        c               : out    vl_logic;
        n               : out    vl_logic;
        z               : out    vl_logic;
        v               : out    vl_logic
    );
end alu32;
