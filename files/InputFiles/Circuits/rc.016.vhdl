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
begin
	net_20 <= in0;
	net_21 <= in1;
	net_22 <= in2;
	net_23 <= in3;
	net_24 <= in4;
	net_25 <= in5;
	net_26 <= in6;
	U32 : XNOR2 port map(A1 => net_25, A2 => net_23, Z => net_13);
	U33 : AND4 port map(A1 => net_24, A2 => net_21, A3 => net_21, A4 => net_26, Z => net_14);
	U36 : OR3 port map(A1 => net_25, A2 => net_21, A3 => net_23, Z => net_15);
	U37 : XNOR2 port map(A1 => net_22, A2 => net_23, Z => net_16);
	U38 : XOR2 port map(A1 => net_24, A2 => net_25, Z => net_17);
	U40 : XOR3 port map(A1 => net_20, A2 => net_21, A3 => net_24, Z => net_18);
	U41 : XNOR2 port map(A1 => net_23, A2 => net_24, Z => net_19);
	U22 : OR2 port map(A1 => net_19, A2 => net_20, Z => net_6);
	U23 : NAND3 port map(A1 => net_13, A2 => net_15, A3 => net_15, Z => net_7);
	U24 : OR3 port map(A1 => net_20, A2 => net_16, A3 => net_20, Z => net_8);
	U25 : OR3 port map(A1 => net_15, A2 => net_23, A3 => net_26, Z => net_9);
	U26 : XOR2 port map(A1 => net_16, A2 => net_13, Z => net_10);
	U28 : NOR2 port map(A1 => net_17, A2 => net_18, Z => net_11);
	U29 : NOT1 port map(A1 => net_14, Z => net_12);
	U12 : NOT1 port map(A1 => net_8, Z => net_2);
	U13 : NOT1 port map(A1 => net_6, Z => net_3);
	U14 : NAND3 port map(A1 => net_7, A2 => net_10, A3 => net_12, Z => net_4);
	U15 : XNOR2 port map(A1 => net_9, A2 => net_11, Z => net_5);
	U2 : OR4 port map(A1 => net_2, A2 => net_3, A3 => net_4, A4 => net_5, Z => net_1);
	U1 : NOT1 port map(A1 => net_1, Z => net_0);
	out0 <= net_0;
end struct;
