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
begin
	net_13 <= in0;
	net_14 <= in1;
	net_15 <= in2;
	net_16 <= in3;
	net_17 <= in4;
	net_18 <= in5;
	net_19 <= in6;
	U13 : XOR2 port map(A1 => net_17, A2 => net_17, Z => net_9);
	U14 : XNOR2 port map(A1 => net_13, A2 => net_14, Z => net_10);
	U15 : NAND2 port map(A1 => net_16, A2 => net_16, Z => net_11);
	U16 : NOT1 port map(A1 => net_16, Z => net_12);
	U8 : NOT1 port map(A1 => net_9, Z => net_4);
	U9 : XOR2 port map(A1 => net_18, A2 => net_19, Z => net_5);
	U10 : XNOR3 port map(A1 => net_15, A2 => net_15, A3 => net_16, Z => net_6);
	U11 : OR3 port map(A1 => net_11, A2 => net_13, A3 => net_15, Z => net_7);
	U12 : XNOR3 port map(A1 => net_14, A2 => net_15, A3 => net_13, Z => net_8);
	U2 : XNOR3 port map(A1 => net_4, A2 => net_6, A3 => net_7, Z => net_1);
	U3 : XOR3 port map(A1 => net_5, A2 => net_10, A3 => net_13, Z => net_2);
	U4 : XOR3 port map(A1 => net_8, A2 => net_12, A3 => net_13, Z => net_3);
	U1 : XOR3 port map(A1 => net_1, A2 => net_2, A3 => net_3, Z => net_0);
	out0 <= net_0;
end struct;
