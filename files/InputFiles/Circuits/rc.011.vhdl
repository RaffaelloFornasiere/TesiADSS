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
	signal net_38		: std_logic;
begin
	net_32 <= in0;
	net_33 <= in1;
	net_34 <= in2;
	net_35 <= in3;
	net_36 <= in4;
	net_37 <= in5;
	net_38 <= in6;
	U32 : NAND4 port map(A1 => net_36, A2 => net_37, A3 => net_36, A4 => net_33, Z => net_22);
	U33 : XNOR3 port map(A1 => net_34, A2 => net_33, A3 => net_38, Z => net_23);
	U34 : NOT1 port map(A1 => net_32, Z => net_24);
	U35 : AND3 port map(A1 => net_32, A2 => net_34, A3 => net_35, Z => net_25);
	U36 : NOR4 port map(A1 => net_36, A2 => net_36, A3 => net_32, A4 => net_38, Z => net_26);
	U37 : NAND3 port map(A1 => net_34, A2 => net_35, A3 => net_35, Z => net_27);
	U38 : XNOR3 port map(A1 => net_32, A2 => net_34, A3 => net_35, Z => net_28);
	U39 : XNOR3 port map(A1 => net_38, A2 => net_33, A3 => net_34, Z => net_29);
	U40 : AND3 port map(A1 => net_35, A2 => net_37, A3 => net_35, Z => net_30);
	U41 : XNOR3 port map(A1 => net_36, A2 => net_38, A3 => net_33, Z => net_31);
	U22 : XOR3 port map(A1 => net_34, A2 => net_28, A3 => net_34, Z => net_12);
	U23 : OR4 port map(A1 => net_25, A2 => net_37, A3 => net_29, A4 => net_35, Z => net_13);
	U24 : XOR3 port map(A1 => net_23, A2 => net_25, A3 => net_27, Z => net_14);
	U25 : XNOR2 port map(A1 => net_22, A2 => net_34, Z => net_15);
	U26 : AND2 port map(A1 => net_32, A2 => net_38, Z => net_16);
	U27 : XNOR3 port map(A1 => net_24, A2 => net_37, A3 => net_32, Z => net_17);
	U28 : XOR3 port map(A1 => net_26, A2 => net_33, A3 => net_38, Z => net_18);
	U29 : XNOR3 port map(A1 => net_36, A2 => net_35, A3 => net_31, Z => net_19);
	U30 : OR2 port map(A1 => net_30, A2 => net_26, Z => net_20);
	U31 : XOR2 port map(A1 => net_33, A2 => net_34, Z => net_21);
	U12 : NAND2 port map(A1 => net_21, A2 => net_28, Z => net_5);
	U13 : OR4 port map(A1 => net_12, A2 => net_17, A3 => net_32, A4 => net_35, Z => net_6);
	U14 : XNOR3 port map(A1 => net_15, A2 => net_15, A3 => net_37, Z => net_7);
	U18 : OR4 port map(A1 => net_13, A2 => net_16, A3 => net_30, A4 => net_38, Z => net_8);
	U19 : XNOR3 port map(A1 => net_20, A2 => net_29, A3 => net_33, Z => net_9);
	U20 : OR2 port map(A1 => net_28, A2 => net_31, Z => net_10);
	U21 : OR3 port map(A1 => net_14, A2 => net_18, A3 => net_27, Z => net_11);
	U2 : OR3 port map(A1 => net_8, A2 => net_9, A3 => net_19, Z => net_1);
	U3 : NOT1 port map(A1 => net_5, Z => net_2);
	U4 : NOT1 port map(A1 => net_7, Z => net_3);
	U5 : OR4 port map(A1 => net_6, A2 => net_10, A3 => net_11, A4 => net_27, Z => net_4);
	U1 : OR4 port map(A1 => net_1, A2 => net_2, A3 => net_3, A4 => net_4, Z => net_0);
	out0 <= net_0;
end struct;
