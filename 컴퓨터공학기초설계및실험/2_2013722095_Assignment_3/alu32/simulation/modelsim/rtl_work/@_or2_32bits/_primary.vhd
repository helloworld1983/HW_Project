library verilog;
use verilog.vl_types.all;
entity \_or2_32bits\ is
    port(
        a               : in     vl_logic_vector(31 downto 0);
        b               : in     vl_logic_vector(31 downto 0);
        y               : out    vl_logic_vector(31 downto 0)
    );
end \_or2_32bits\;
