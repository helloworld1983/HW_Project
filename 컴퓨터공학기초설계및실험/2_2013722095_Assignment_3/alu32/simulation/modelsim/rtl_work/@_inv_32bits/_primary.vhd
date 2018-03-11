library verilog;
use verilog.vl_types.all;
entity \_inv_32bits\ is
    port(
        a               : in     vl_logic_vector(31 downto 0);
        y               : out    vl_logic_vector(31 downto 0)
    );
end \_inv_32bits\;
