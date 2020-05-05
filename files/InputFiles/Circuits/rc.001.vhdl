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
begin
	net_12 <= in0;
	net_13 <= in1;
	net_14 <= in2;
	net_15 <= in3;
	net_16 <= in4;
	net_17 <= in5;
	net_18 <= in6;
	U22 : XOR2 port map(A1 => net_17, A2 => net_18, Z => net_8);
	U23 : XNOR3 port map(A1 => net_13, A2 => net_13, A3 => net_16, Z => net_9);
	U24 : XOR3 port map(A1 => net_13, A2 => net_14, A3 => net_17, Z => net_10);
	U26 : XOR3 port map(A1 => net_14, A2 => net_16, A3 => net_18, Z => net_11);
	U14 : XOR2 port map(A1 => net_10, A2 => net_15, Z => net_4);
	U15 : NOT1 port map(A1 => net_11, Z => net_5);
	U16 : XOR2 port map(A1 => net_9, A2 => net_12, Z => net_6);
	U18 : NOT1 port map(A1 => net_8, Z => net_7);
	U9 : XNOR2 port map(A1 => net_4, A2 => net_7, Z => net_2);
	U10 : NOR2 port map(A1 => net_5, A2 => net_6, Z => net_3);
	U2 : NAND2 port map(A1 => net_2, A2 => net_3, Z => net_1);
	U1 : NOT1 port map(A1 => net_1, Z => net_0);
	out0 <= net_0;
end struct;
