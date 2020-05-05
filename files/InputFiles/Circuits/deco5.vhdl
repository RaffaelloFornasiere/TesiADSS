library ieee;
use ieee.std_logic_1164.all;

----------------------------------------------------------------------
entity deco5 is
	port (
		input		: in  std_logic_vector(4 downto 0);
		output		: out std_logic_vector(0 to 31)
	);
end deco5;
----------------------------------------------------------------------

----------------------------------------------------------------------
architecture struct of deco5 is
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
	signal net_48		: std_logic;
	signal net_49		: std_logic;
	signal net_50		: std_logic;
	signal net_51		: std_logic;
	signal net_52		: std_logic;
	signal net_53		: std_logic;
	signal net_54		: std_logic;
	signal net_55		: std_logic;
	signal net_56		: std_logic;
	signal net_57		: std_logic;
	signal net_58		: std_logic;
	signal net_59		: std_logic;
	signal net_60		: std_logic;
	signal net_61		: std_logic;
	signal net_62		: std_logic;
	signal net_63		: std_logic;
	signal net_64		: std_logic;
	signal net_65		: std_logic;
begin
	net_0 <= input(0);
	net_1 <= input(1);
	net_2 <= input(2);
	net_3 <= input(3);
	net_4 <= input(4);
	I1 : INV port map (A => input(0), Z => net_5);
	I2 : INV port map (A => input(1), Z => net_6);
	I3 : INV port map (A => input(2), Z => net_7);
	I4 : INV port map (A => input(3), Z => net_8);
	I5 : INV port map (A => input(4), Z => net_9);

	U1 : NOR2 port map (A1 => net_4, A2 => net_3, Z => net_10);
	U2 : NOR2 port map (A1 => net_2, A2 => net_1, Z => net_11);
	U3 : NAND2 port map (A1 => net_10, A2 => net_11, Z => net_12);
	U4 : NOR2 port map (A1 => net_12, A2 => net_0, Z => net_13);
	output(0) <= net_13;

	U5 : NOR2 port map (A1 => net_12, A2 => net_5, Z => net_14);
	output(1) <= net_14;

	U6 : NOR2 port map (A1 => net_2, A2 => net_6, Z => net_15);
	U7 : NAND2 port map (A1 => net_10, A2 => net_15, Z => net_16);
	U8 : NOR2 port map (A1 => net_16, A2 => net_0, Z => net_17);
	output(2) <= net_17;

	U9 : NOR2 port map (A1 => net_16, A2 => net_5, Z => net_18);
	output(3) <= net_18;

	U10 : NOR2 port map (A1 => net_7, A2 => net_1, Z => net_19);
	U11 : NAND2 port map (A1 => net_10, A2 => net_19, Z => net_20);
	U12 : NOR2 port map (A1 => net_20, A2 => net_0, Z => net_21);
	output(4) <= net_21;

	U13 : NOR2 port map (A1 => net_20, A2 => net_5, Z => net_22);
	output(5) <= net_22;

	U14 : NOR2 port map (A1 => net_7, A2 => net_6, Z => net_23);
	U15 : NAND2 port map (A1 => net_10, A2 => net_23, Z => net_24);
	U16 : NOR2 port map (A1 => net_24, A2 => net_0, Z => net_25);
	output(6) <= net_25;

	U17 : NOR2 port map (A1 => net_24, A2 => net_5, Z => net_26);
	output(7) <= net_26;

	U18 : NOR2 port map (A1 => net_4, A2 => net_8, Z => net_27);
	U19 : NAND2 port map (A1 => net_27, A2 => net_11, Z => net_28);
	U20 : NOR2 port map (A1 => net_28, A2 => net_0, Z => net_29);
	output(8) <= net_29;

	U21 : NOR2 port map (A1 => net_28, A2 => net_5, Z => net_30);
	output(9) <= net_30;

	U22 : NAND2 port map (A1 => net_27, A2 => net_15, Z => net_31);
	U23 : NOR2 port map (A1 => net_31, A2 => net_0, Z => net_32);
	output(10) <= net_32;

	U24 : NOR2 port map (A1 => net_31, A2 => net_5, Z => net_33);
	output(11) <= net_33;

	U25 : NAND2 port map (A1 => net_27, A2 => net_19, Z => net_34);
	U26 : NOR2 port map (A1 => net_34, A2 => net_0, Z => net_35);
	output(12) <= net_35;

	U27 : NOR2 port map (A1 => net_34, A2 => net_5, Z => net_36);
	output(13) <= net_36;

	U28 : NAND2 port map (A1 => net_27, A2 => net_23, Z => net_37);
	U29 : NOR2 port map (A1 => net_37, A2 => net_0, Z => net_38);
	output(14) <= net_38;

	U30 : NOR2 port map (A1 => net_37, A2 => net_5, Z => net_39);
	output(15) <= net_39;

	U31 : NOR2 port map (A1 => net_9, A2 => net_3, Z => net_40);
	U32 : NAND2 port map (A1 => net_40, A2 => net_11, Z => net_41);
	U33 : NOR2 port map (A1 => net_41, A2 => net_0, Z => net_42);
	output(16) <= net_42;

	U34 : NOR2 port map (A1 => net_41, A2 => net_5, Z => net_43);
	output(17) <= net_43;

	U35 : NAND2 port map (A1 => net_40, A2 => net_15, Z => net_44);
	U36 : NOR2 port map (A1 => net_44, A2 => net_0, Z => net_45);
	output(18) <= net_45;

	U37 : NOR2 port map (A1 => net_44, A2 => net_5, Z => net_46);
	output(19) <= net_46;

	U38 : NAND2 port map (A1 => net_40, A2 => net_19, Z => net_47);
	U39 : NOR2 port map (A1 => net_47, A2 => net_0, Z => net_48);
	output(20) <= net_48;

	U40 : NOR2 port map (A1 => net_47, A2 => net_5, Z => net_49);
	output(21) <= net_49;

	U41 : NAND2 port map (A1 => net_40, A2 => net_23, Z => net_50);
	U42 : NOR2 port map (A1 => net_50, A2 => net_0, Z => net_51);
	output(22) <= net_51;

	U43 : NOR2 port map (A1 => net_50, A2 => net_5, Z => net_52);
	output(23) <= net_52;

	U44 : NOR2 port map (A1 => net_9, A2 => net_8, Z => net_53);
	U45 : NAND2 port map (A1 => net_53, A2 => net_11, Z => net_54);
	U46 : NOR2 port map (A1 => net_54, A2 => net_0, Z => net_55);
	output(24) <= net_55;

	U47 : NOR2 port map (A1 => net_54, A2 => net_5, Z => net_56);
	output(25) <= net_56;

	U48 : NAND2 port map (A1 => net_53, A2 => net_15, Z => net_57);
	U49 : NOR2 port map (A1 => net_57, A2 => net_0, Z => net_58);
	output(26) <= net_58;

	U50 : NOR2 port map (A1 => net_57, A2 => net_5, Z => net_59);
	output(27) <= net_59;

	U51 : NAND2 port map (A1 => net_53, A2 => net_19, Z => net_60);
	U52 : NOR2 port map (A1 => net_60, A2 => net_0, Z => net_61);
	output(28) <= net_61;

	U53 : NOR2 port map (A1 => net_60, A2 => net_5, Z => net_62);
	output(29) <= net_62;

	U54 : NAND2 port map (A1 => net_53, A2 => net_23, Z => net_63);
	U55 : NOR2 port map (A1 => net_63, A2 => net_0, Z => net_64);
	output(30) <= net_64;

	U56 : NOR2 port map (A1 => net_63, A2 => net_5, Z => net_65);
	output(31) <= net_65;
end struct;
----------------------------------------------------------------------
