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
begin
	net_33 <= in0;
	net_34 <= in1;
	net_35 <= in2;
	net_36 <= in3;
	net_37 <= in4;
	net_38 <= in5;
	net_39 <= in6;
	U29 : AND4 port map(A1 => net_35, A2 => net_33, A3 => net_37, A4 => net_39, Z => net_27);
	U30 : NOT1 port map(A1 => net_39, Z => net_28);
	U31 : AND4 port map(A1 => net_34, A2 => net_33, A3 => net_35, A4 => net_39, Z => net_29);
	U32 : NOR4 port map(A1 => net_38, A2 => net_37, A3 => net_35, A4 => net_34, Z => net_30);
	U33 : XOR3 port map(A1 => net_34, A2 => net_39, A3 => net_38, Z => net_31);
	U34 : NAND2 port map(A1 => net_33, A2 => net_34, Z => net_32);
	U23 : NOR3 port map(A1 => net_33, A2 => net_29, A3 => net_37, Z => net_21);
	U24 : NOT1 port map(A1 => net_36, Z => net_22);
	U25 : XNOR2 port map(A1 => net_30, A2 => net_32, Z => net_23);
	U26 : XNOR3 port map(A1 => net_35, A2 => net_38, A3 => net_39, Z => net_24);
	U27 : NOT1 port map(A1 => net_28, Z => net_25);
	U28 : NOT1 port map(A1 => net_35, Z => net_26);
	U17 : NOT1 port map(A1 => net_27, Z => net_15);
	U18 : AND3 port map(A1 => net_34, A2 => net_37, A3 => net_28, Z => net_16);
	U19 : NAND3 port map(A1 => net_34, A2 => net_29, A3 => net_33, Z => net_17);
	U20 : AND3 port map(A1 => net_22, A2 => net_28, A3 => net_36, Z => net_18);
	U21 : XOR3 port map(A1 => net_32, A2 => net_36, A3 => net_38, Z => net_19);
	U22 : AND3 port map(A1 => net_28, A2 => net_34, A3 => net_29, Z => net_20);
	U12 : NAND4 port map(A1 => net_19, A2 => net_24, A3 => net_29, A4 => net_19, Z => net_11);
	U13 : XNOR3 port map(A1 => net_15, A2 => net_16, A3 => net_38, Z => net_12);
	U14 : NOT1 port map(A1 => net_21, Z => net_13);
	U16 : NAND3 port map(A1 => net_33, A2 => net_38, A3 => net_24, Z => net_14);
	U7 : NOR2 port map(A1 => net_11, A2 => net_31, Z => net_6);
	U8 : NOT1 port map(A1 => net_25, Z => net_7);
	U9 : XOR2 port map(A1 => net_14, A2 => net_33, Z => net_8);
	U10 : NOT1 port map(A1 => net_12, Z => net_9);
	U11 : XOR2 port map(A1 => net_23, A2 => net_37, Z => net_10);
	U1 : NAND4 port map(A1 => net_18, A2 => net_20, A3 => net_39, A4 => net_34, Z => net_0);
	U2 : NOT1 port map(A1 => net_10, Z => net_1);
	U3 : OR3 port map(A1 => net_6, A2 => net_13, A3 => net_26, Z => net_2);
	U4 : OR3 port map(A1 => net_17, A2 => net_31, A3 => net_31, Z => net_3);
	U5 : NOT1 port map(A1 => net_8, Z => net_4);
	U6 : NOT1 port map(A1 => net_7, Z => net_5);
	out0 <= net_0;
	out1 <= net_3;
	out2 <= net_5;
	out3 <= net_2;
	out4 <= net_4;
	out5 <= net_1;
	out6 <= net_9;
end struct;
