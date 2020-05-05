library ieee;
use ieee.std_logic_1164.all;

----------------------------------------------------------------------
entity deco3 is
	port (
		input		: in  std_logic_vector(2 downto 0);
		output		: out std_logic_vector(0 to 7)
	);
end deco3;
----------------------------------------------------------------------

----------------------------------------------------------------------
architecture struct of deco3 is
	signal net_0		: std_logic;
	signal net_1		: std_logic;
	signal net_2		: std_logic;
	signal net_3		: std_logic;
	signal net_4		: std_logic;
	signal net_5		: std_logic;
	signal net_6		: std_logic;
	signal net_7		: std_logic;
	signal net_8		: std_logic;
	signal net_9		: std_logic;
	signal net_10		: std_logic;
	signal net_11		: std_logic;
	signal net_12		: std_logic;
	signal net_13		: std_logic;
	signal net_14		: std_logic;
	signal net_15		: std_logic;
	signal net_16		: std_logic;
	signal net_17		: std_logic;
begin
	net_0 <= input(0);
	net_1 <= input(1);
	net_2 <= input(2);
	I1 : INV port map (A => input(0), Z => net_3);
	I2 : INV port map (A => input(1), Z => net_4);
	I3 : INV port map (A => input(2), Z => net_5);

	U1 : AND2 port map (A1 => net_5, A2 => net_4, Z => net_6);
	U2 : AND2 port map (A1 => net_6, A2 => net_3, Z => net_7);
	output(0) <= net_7;

	U3 : AND2 port map (A1 => net_6, A2 => net_0, Z => net_8);
	output(1) <= net_8;

	U4 : AND2 port map (A1 => net_5, A2 => net_1, Z => net_9);
	U5 : AND2 port map (A1 => net_9, A2 => net_3, Z => net_10);
	output(2) <= net_10;

	U6 : AND2 port map (A1 => net_9, A2 => net_0, Z => net_11);
	output(3) <= net_11;

	U7 : AND2 port map (A1 => net_2, A2 => net_4, Z => net_12);
	U8 : AND2 port map (A1 => net_12, A2 => net_3, Z => net_13);
	output(4) <= net_13;

	U9 : AND2 port map (A1 => net_12, A2 => net_0, Z => net_14);
	output(5) <= net_14;

	U10 : AND2 port map (A1 => net_2, A2 => net_1, Z => net_15);
	U11 : AND2 port map (A1 => net_15, A2 => net_3, Z => net_16);
	output(6) <= net_16;

	U12 : AND2 port map (A1 => net_15, A2 => net_0, Z => net_17);
	output(7) <= net_17;
end struct;
----------------------------------------------------------------------
