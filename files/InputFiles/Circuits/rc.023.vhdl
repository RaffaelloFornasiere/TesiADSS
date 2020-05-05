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
		out0		: out std_logic;
		out1		: out std_logic;
		out2		: out std_logic;
		out3		: out std_logic;
		out4		: out std_logic;
		out5		: out std_logic;
		out6		: out std_logic
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
	signal net_39		: std_logic;
	signal net_40		: std_logic;
	signal net_41		: std_logic;
	signal net_42		: std_logic;
	signal net_43		: std_logic;
	signal net_44		: std_logic;
	signal net_45		: std_logic;
begin
	net_39 <= in0;
	net_40 <= in1;
	net_41 <= in2;
	net_42 <= in3;
	net_43 <= in4;
	net_44 <= in5;
	net_45 <= in6;
	U36 : NOR2 port map(A1 => net_44, A2 => net_44, Z => net_35);
	U37 : XNOR2 port map(A1 => net_39, A2 => net_45, Z => net_36);
	U38 : NOT1 port map(A1 => net_42, Z => net_37);
	U39 : NOT1 port map(A1 => net_40, Z => net_38);
	U30 : AND4 port map(A1 => net_35, A2 => net_42, A3 => net_36, A4 => net_41, Z => net_29);
	U31 : OR2 port map(A1 => net_44, A2 => net_40, Z => net_30);
	U32 : NOR2 port map(A1 => net_40, A2 => net_42, Z => net_31);
	U33 : NAND2 port map(A1 => net_41, A2 => net_43, Z => net_32);
	U34 : NOT1 port map(A1 => net_35, Z => net_33);
	U35 : OR2 port map(A1 => net_41, A2 => net_37, Z => net_34);
	U26 : XNOR3 port map(A1 => net_30, A2 => net_38, A3 => net_35, Z => net_25);
	U27 : NOT1 port map(A1 => net_43, Z => net_26);
	U28 : XNOR3 port map(A1 => net_37, A2 => net_34, A3 => net_32, Z => net_27);
	U29 : XOR3 port map(A1 => net_32, A2 => net_36, A3 => net_36, Z => net_28);
	U19 : XNOR2 port map(A1 => net_25, A2 => net_25, Z => net_18);
	U20 : OR4 port map(A1 => net_42, A2 => net_34, A3 => net_25, A4 => net_45, Z => net_19);
	U21 : XOR3 port map(A1 => net_34, A2 => net_42, A3 => net_34, Z => net_20);
	U22 : XOR2 port map(A1 => net_44, A2 => net_40, Z => net_21);
	U23 : XOR3 port map(A1 => net_33, A2 => net_45, A3 => net_37, Z => net_22);
	U24 : AND3 port map(A1 => net_25, A2 => net_40, A3 => net_44, Z => net_23);
	U25 : NOT1 port map(A1 => net_32, Z => net_24);
	U14 : XOR3 port map(A1 => net_27, A2 => net_36, A3 => net_30, Z => net_13);
	U15 : XOR3 port map(A1 => net_39, A2 => net_41, A3 => net_21, Z => net_14);
	U16 : NOR4 port map(A1 => net_36, A2 => net_26, A3 => net_26, A4 => net_32, Z => net_15);
	U17 : NOR3 port map(A1 => net_18, A2 => net_27, A3 => net_30, Z => net_16);
	U18 : OR3 port map(A1 => net_38, A2 => net_45, A3 => net_30, Z => net_17);
	U8 : AND3 port map(A1 => net_14, A2 => net_24, A3 => net_26, Z => net_7);
	U9 : XNOR3 port map(A1 => net_15, A2 => net_23, A3 => net_31, Z => net_8);
	U10 : NOT1 port map(A1 => net_39, Z => net_9);
	U11 : XNOR3 port map(A1 => net_37, A2 => net_37, A3 => net_26, Z => net_10);
	U12 : NOR4 port map(A1 => net_45, A2 => net_17, A3 => net_13, A4 => net_23, Z => net_11);
	U13 : NAND3 port map(A1 => net_41, A2 => net_18, A3 => net_35, Z => net_12);
	U1 : OR4 port map(A1 => net_7, A2 => net_11, A3 => net_17, A4 => net_37, Z => net_0);
	U2 : OR4 port map(A1 => net_13, A2 => net_20, A3 => net_22, A4 => net_30, Z => net_1);
	U3 : NOR4 port map(A1 => net_8, A2 => net_16, A3 => net_19, A4 => net_21, Z => net_2);
	U4 : AND4 port map(A1 => net_28, A2 => net_29, A3 => net_26, A4 => net_13, Z => net_3);
	U5 : NOT1 port map(A1 => net_12, Z => net_4);
	U6 : NAND2 port map(A1 => net_21, A2 => net_25, Z => net_5);
	U7 : XNOR2 port map(A1 => net_9, A2 => net_10, Z => net_6);
	out0 <= net_4;
	out1 <= net_2;
	out2 <= net_6;
	out3 <= net_5;
	out4 <= net_3;
	out5 <= net_0;
	out6 <= net_1;
end struct;
