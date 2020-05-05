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
begin
	net_24 <= in0;
	net_25 <= in1;
	net_26 <= in2;
	net_27 <= in3;
	net_28 <= in4;
	net_29 <= in5;
	net_30 <= in6;
	U17 : XOR3 port map(A1 => net_30, A2 => net_28, A3 => net_27, Z => net_16);
	U18 : NOT1 port map(A1 => net_24, Z => net_17);
	U19 : NAND4 port map(A1 => net_29, A2 => net_29, A3 => net_28, A4 => net_24, Z => net_18);
	U20 : NOT1 port map(A1 => net_28, Z => net_19);
	U21 : XNOR3 port map(A1 => net_27, A2 => net_30, A3 => net_24, Z => net_20);
	U22 : NAND4 port map(A1 => net_24, A2 => net_27, A3 => net_25, A4 => net_29, Z => net_21);
	U23 : OR4 port map(A1 => net_25, A2 => net_30, A3 => net_25, A4 => net_29, Z => net_22);
	U24 : XOR2 port map(A1 => net_24, A2 => net_28, Z => net_23);
	U11 : NOT1 port map(A1 => net_30, Z => net_10);
	U12 : AND4 port map(A1 => net_23, A2 => net_30, A3 => net_24, A4 => net_22, Z => net_11);
	U13 : NOR4 port map(A1 => net_17, A2 => net_26, A3 => net_16, A4 => net_20, Z => net_12);
	U14 : XOR3 port map(A1 => net_16, A2 => net_17, A3 => net_27, Z => net_13);
	U15 : OR4 port map(A1 => net_26, A2 => net_26, A3 => net_28, A4 => net_23, Z => net_14);
	U16 : OR3 port map(A1 => net_22, A2 => net_24, A3 => net_29, Z => net_15);
	U5 : NOT1 port map(A1 => net_13, Z => net_4);
	U6 : XOR3 port map(A1 => net_25, A2 => net_27, A3 => net_27, Z => net_5);
	U7 : NAND4 port map(A1 => net_11, A2 => net_20, A3 => net_25, A4 => net_29, Z => net_6);
	U8 : OR4 port map(A1 => net_17, A2 => net_13, A3 => net_15, A4 => net_23, Z => net_7);
	U9 : NOT1 port map(A1 => net_21, Z => net_8);
	U10 : NOR2 port map(A1 => net_14, A2 => net_15, Z => net_9);
	U1 : XOR3 port map(A1 => net_4, A2 => net_6, A3 => net_18, Z => net_0);
	U2 : NOR3 port map(A1 => net_12, A2 => net_15, A3 => net_22, Z => net_1);
	U3 : XNOR3 port map(A1 => net_5, A2 => net_7, A3 => net_9, Z => net_2);
	U4 : NOR2 port map(A1 => net_16, A2 => net_19, Z => net_3);
	out0 <= net_2;
	out1 <= net_1;
	out2 <= net_12;
	out3 <= net_8;
	out4 <= net_10;
	out5 <= net_3;
	out6 <= net_0;
end struct;
