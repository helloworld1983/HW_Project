library verilog;
use verilog.vl_types.all;
entity mx8_32bits is
    port(
        a               : in     vl_logic_vector(31 downto 0);
        b               : in     vl_logic_vector(31 downto 0);
        c               : in     vl_logic_vector(31 downto 0);
        d               : in     vl_logic_vector(31 downto 0);
        e               : in     vl_logic_vector(31 downto 0);
        f               : in     vl_logic_vector(31 downto 0);
        g               : in     vl_logic_vector(31 downto 0);
        h               : in     vl_logic_vector(31 downto 0);
        s2              : in     vl_logic;
        s1              : in     vl_logic;
        s0              : in     vl_logic;
        y               : out    vl_logic_vector(31 downto 0)
    );
end mx8_32bits;
