library verilog;
use verilog.vl_types.all;
entity \_xor2_32bits\ is
    port(
        a               : in     vl_logic_vector(31 downto 0);
        b               : in     vl_logic_vector(31 downto 0);
        y               : out    vl_logic_vector(31 downto 0)
    );
end \_xor2_32bits\;
