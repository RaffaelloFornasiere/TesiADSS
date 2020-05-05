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
begin
	net_17 <= in0;
	net_18 <= in1;
	net_19 <= in2;
	net_20 <= in3;
	net_21 <= in4;
	net_22 <= in5;
	net_23 <= in6;
	U18 : AND3 port map(A1 => net_20, A2 => net_22, A3 => net_23, Z => net_13);
	U19 : NOT1 port map(A1 => net_17, Z => net_14);
	U20 : NOR3 port map(A1 => net_22, A2 => net_23, A3 => net_20, Z => net_15);
	U21 : AND2 port map(A1 => net_19, A2 => net_20, Z => net_16);
	U13 : XOR3 port map(A1 => net_18, A2 => net_15, A3 => net_16, Z => net_8);
	U14 : XOR3 port map(A1 => net_17, A2 => net_21, A3 => net_19, Z => net_9);
	U15 : NOT1 port map(A1 => net_22, Z => net_10);
	U16 : XOR3 port map(A1 => net_21, A2 => net_17, A3 => net_13, Z => net_11);
	U17 : OR2 port map(A1 => net_14, A2 => net_17, Z => net_12);
	U8 : NAND4 port map(A1 => net_13, A2 => net_21, A3 => net_10, A4 => net_11, Z => net_3);
	U9 : XOR2 port map(A1 => net_11, A2 => net_15, Z => net_4);
	U10 : AND2 port map(A1 => net_8, A2 => net_18, Z => net_5);
	U11 : OR4 port map(A1 => net_9, A2 => net_12, A3 => net_16, A4 => net_12, Z => net_6);
	U12 : NOT1 port map(A1 => net_10, Z => net_7);
	U2 : XNOR2 port map(A1 => net_4, A2 => net_5, Z => net_1);
	U3 : AND3 port map(A1 => net_3, A2 => net_6, A3 => net_7, Z => net_2);
	U1 : AND2 port map(A1 => net_1, A2 => net_2, Z => net_0);
	out0 <= net_0;
end struct;
