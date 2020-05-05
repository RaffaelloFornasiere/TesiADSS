----------------------------------------------------------------------
entity randomcircuit is
	port (
		in0		: in  std_logic;
		in1		: in  std_logic;
		in2		: in  std_logic;
		in3		: in  std_logic;
		in4		: in  std_logic;
		in5		: in  std_logic;
		in6		: in  std_logic;
		out0		: out std_logic
	);
end randomcircuit;
----------------------------------------------------------------------

----------------------------------------------------------------------
architecture struct of randomcircuit is
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
begin
	net_31 <= in0;
	net_32 <= in1;
	net_33 <= in2;
	net_34 <= in3;
	net_35 <= in4;
	net_36 <= in5;
	net_37 <= in6;
	U31 : OR4 port map(A1 => net_36, A2 => net_33, A3 => net_33, A4 => net_32, Z => net_24);
	U32 : NAND2 port map(A1 => net_37, A2 => net_33, Z => net_25);
	U33 : AND4 port map(A1 => net_35, A2 => net_36, A3 => net_34, A4 => net_35, Z => net_26);
	U34 : NOT1 port map(A1 => net_32, Z => net_27);
	U35 : NAND3 port map(A1 => net_36, A2 => net_37, A3 => net_31, Z => net_28);
	U36 : NAND4 port map(A1 => net_31, A2 => net_34, A3 => net_35, A4 => net_36, Z => net_29);
	U37 : XOR3 port map(A1 => net_34, A2 => net_31, A3 => net_33, Z => net_30);
	U26 : NOT1 port map(A1 => net_25, Z => net_19);
	U27 : NOR2 port map(A1 => net_28, A2 => net_31, Z => net_20);
	U28 : XOR2 port map(A1 => net_33, A2 => net_37, Z => net_21);
	U29 : NOT1 port map(A1 => net_35, Z => net_22);
	U30 : XNOR2 port map(A1 => net_26, A2 => net_29, Z => net_23);
	U18 : NOT1 port map(A1 => net_30, Z => net_11);
	U19 : AND4 port map(A1 => net_36, A2 => net_22, A3 => net_21, A4 => net_21, Z => net_12);
	U20 : NOT1 port map(A1 => net_22, Z => net_13);
	U21 : NOT1 port map(A1 => net_29, Z => net_14);
	U22 : NOT1 port map(A1 => net_22, Z => net_15);
	U23 : NAND3 port map(A1 => net_26, A2 => net_19, A3 => net_22, Z => net_16);
	U24 : AND3 port map(A1 => net_19, A2 => net_23, A3 => net_36, Z => net_17);
	U25 : AND2 port map(A1 => net_24, A2 => net_26, Z => net_18);
	U13 : XOR3 port map(A1 => net_27, A2 => net_28, A3 => net_32, Z => net_6);
	U14 : NOT1 port map(A1 => net_20, Z => net_7);
	U15 : NOT1 port map(A1 => net_37, Z => net_8);
	U16 : AND3 port map(A1 => net_13, A2 => net_18, A3 => net_32, Z => net_9);
	U17 : XNOR3 port map(A1 => net_14, A2 => net_15, A3 => net_17, Z => net_10);
	U9 : NAND4 port map(A1 => net_11, A2 => net_12, A3 => net_16, A4 => net_21, Z => net_3);
	U10 : XNOR3 port map(A1 => net_30, A2 => net_32, A3 => net_17, Z => net_4);
	U11 : XNOR2 port map(A1 => net_7, A2 => net_10, Z => net_5);
	U2 : AND3 port map(A1 => net_3, A2 => net_4, A3 => net_5, Z => net_1);
	U3 : NAND3 port map(A1 => net_6, A2 => net_8, A3 => net_9, Z => net_2);
	U1 : OR2 port map(A1 => net_1, A2 => net_2, Z => net_0);
	out0 <= net_0;
end struct;
