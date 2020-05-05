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
	U23 : XNOR3 port map(A1 => net_23, A2 => net_23, A3 => net_22, Z => net_16);
	U24 : OR4 port map(A1 => net_25, A2 => net_25, A3 => net_26, A4 => net_22, Z => net_17);
	U25 : AND2 port map(A1 => net_24, A2 => net_24, Z => net_18);
	U26 : AND4 port map(A1 => net_20, A2 => net_22, A3 => net_25, A4 => net_23, Z => net_19);
	U15 : AND4 port map(A1 => net_25, A2 => net_19, A3 => net_21, A4 => net_22, Z => net_8);
	U16 : NAND3 port map(A1 => net_16, A2 => net_21, A3 => net_24, Z => net_9);
	U17 : XNOR3 port map(A1 => net_26, A2 => net_17, A3 => net_25, Z => net_10);
	U18 : NOR3 port map(A1 => net_17, A2 => net_20, A3 => net_25, Z => net_11);
	U19 : NOR2 port map(A1 => net_16, A2 => net_20, Z => net_12);
	U20 : XNOR2 port map(A1 => net_21, A2 => net_21, Z => net_13);
	U21 : XNOR2 port map(A1 => net_20, A2 => net_20, Z => net_14);
	U22 : AND4 port map(A1 => net_25, A2 => net_26, A3 => net_26, A4 => net_25, Z => net_15);
	U10 : OR4 port map(A1 => net_18, A2 => net_15, A3 => net_23, A4 => net_15, Z => net_3);
	U11 : XOR2 port map(A1 => net_8, A2 => net_11, Z => net_4);
	U12 : XNOR3 port map(A1 => net_15, A2 => net_19, A3 => net_26, Z => net_5);
	U13 : AND3 port map(A1 => net_14, A2 => net_10, A3 => net_18, Z => net_6);
	U14 : XOR3 port map(A1 => net_10, A2 => net_12, A3 => net_22, Z => net_7);
	U2 : AND4 port map(A1 => net_3, A2 => net_4, A3 => net_5, A4 => net_9, Z => net_1);
	U3 : XOR3 port map(A1 => net_6, A2 => net_7, A3 => net_13, Z => net_2);
	U1 : XNOR2 port map(A1 => net_1, A2 => net_2, Z => net_0);
	out0 <= net_0;
end struct;
