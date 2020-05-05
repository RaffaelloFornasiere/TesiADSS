library ieee;
use ieee.std_logic_1164.all;

----------------------------------------------------------------------
entity deco4 is
	port (
		input		: in  std_logic_vector(3 downto 0);
		output		: out std_logic_vector(0 to 15)
	);
end deco4;
----------------------------------------------------------------------

----------------------------------------------------------------------
architecture struct of deco4 is
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
	signal net_18		: std_logic;
	signal net_19		: std_logic;
	signal net_20		: std_logic;
	signal net_21		: std_logic;
	signal net_22		: std_logic;
	signal net_23		: std_logic;
	signal net_24		: std_logic;
	signal net_25		: std_logic;
	signal net_26		: std_logic;
	signal net_27		: std_logic;
	signal net_28		: std_logic;
	signal net_29		: std_logic;
	signal net_30		: std_logic;
	signal net_31		: std_logic;
	signal net_32		: std_logic;
	signal net_33		: std_logic;
	signal net_34		: std_logic;
	signal net_35		: std_logic;
	signal net_36		: std_logic;
	signal net_37		: std_logic;
	signal net_38		: std_logic;
	signal net_39		: std_logic;
	signal net_40		: std_logic;
	signal net_41		: std_logic;
	signal net_42		: std_logic;
	signal net_43		: std_logic;
	signal net_44		: std_logic;
	signal net_45		: std_logic;
	signal net_46		: std_logic;
	signal net_47		: std_logic;
begin
	net_0 <= input(0);
	net_1 <= input(1);
	net_2 <= input(2);
	net_3 <= input(3);
	I1 : INV port map (A => input(0), Z => net_4);
	I2 : INV port map (A => input(1), Z => net_5);
	I3 : INV port map (A => input(2), Z => net_6);
	I4 : INV port map (A => input(3), Z => net_7);

	U1 : OR2 port map (A1 => net_3, A2 => net_2, Z => net_8);
	U2 : OR2 port map (A1 => net_1, A2 => net_0, Z => net_9);
	U3 : OR2 port map (A1 => net_8, A2 => net_9, Z => net_10);
	I5 : INV port map (A => net_10, Z => net_11);
	output(0) <= net_11;

	U4 : OR2 port map (A1 => net_1, A2 => net_4, Z => net_12);
	U5 : OR2 port map (A1 => net_8, A2 => net_12, Z => net_13);
	I6 : INV port map (A => net_13, Z => net_14);
	output(1) <= net_14;

	U6 : OR2 port map (A1 => net_5, A2 => net_0, Z => net_15);
	U7 : OR2 port map (A1 => net_8, A2 => net_15, Z => net_16);
	I7 : INV port map (A => net_16, Z => net_17);
	output(2) <= net_17;

	U8 : OR2 port map (A1 => net_5, A2 => net_4, Z => net_18);
	U9 : OR2 port map (A1 => net_8, A2 => net_18, Z => net_19);
	I8 : INV port map (A => net_19, Z => net_20);
	output(3) <= net_20;

	U10 : OR2 port map (A1 => net_3, A2 => net_6, Z => net_21);
	U11 : OR2 port map (A1 => net_21, A2 => net_9, Z => net_22);
	I9 : INV port map (A => net_22, Z => net_23);
	output(4) <= net_23;

	U12 : OR2 port map (A1 => net_21, A2 => net_12, Z => net_24);
	I10 : INV port map (A => net_24, Z => net_25);
	output(5) <= net_25;

	U13 : OR2 port map (A1 => net_21, A2 => net_15, Z => net_26);
	I11 : INV port map (A => net_26, Z => net_27);
	output(6) <= net_27;

	U14 : OR2 port map (A1 => net_21, A2 => net_18, Z => net_28);
	I12 : INV port map (A => net_28, Z => net_29);
	output(7) <= net_29;

	U15 : OR2 port map (A1 => net_7, A2 => net_2, Z => net_30);
	U16 : OR2 port map (A1 => net_30, A2 => net_9, Z => net_31);
	I13 : INV port map (A => net_31, Z => net_32);
	output(8) <= net_32;

	U17 : OR2 port map (A1 => net_30, A2 => net_12, Z => net_33);
	I14 : INV port map (A => net_33, Z => net_34);
	output(9) <= net_34;

	U18 : OR2 port map (A1 => net_30, A2 => net_15, Z => net_35);
	I15 : INV port map (A => net_35, Z => net_36);
	output(10) <= net_36;

	U19 : OR2 port map (A1 => net_30, A2 => net_18, Z => net_37);
	I16 : INV port map (A => net_37, Z => net_38);
	output(11) <= net_38;

	U20 : OR2 port map (A1 => net_7, A2 => net_6, Z => net_39);
	U21 : OR2 port map (A1 => net_39, A2 => net_9, Z => net_40);
	I17 : INV port map (A => net_40, Z => net_41);
	output(12) <= net_41;

	U22 : OR2 port map (A1 => net_39, A2 => net_12, Z => net_42);
	I18 : INV port map (A => net_42, Z => net_43);
	output(13) <= net_43;

	U23 : OR2 port map (A1 => net_39, A2 => net_15, Z => net_44);
	I19 : INV port map (A => net_44, Z => net_45);
	output(14) <= net_45;

	U24 : OR2 port map (A1 => net_39, A2 => net_18, Z => net_46);
	I20 : INV port map (A => net_46, Z => net_47);
	output(15) <= net_47;
end struct;
----------------------------------------------------------------------
