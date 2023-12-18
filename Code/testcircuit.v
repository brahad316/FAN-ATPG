module testcircuit (N1,N2,N3,N4,N5,N6,N29);

input N1,N2,N3,N4,N5,N6;

output N29;

wire N11,N14,N15,N16,N22,N23,N24,N25,N27,N28;

or OR2_1 (N11, N3, N4);
xor XOR2_1 (N22, N5, N6);
nor NOR2_1 (N25, N1, N2);
and AND2_1 (N14, N3, N11);
and AND2_2 (N15, N11, N4);
or OR2_2 (N16, N14, N15);
or OR2_3 (N24, N16, N2);
or OR2_4 (N23, N2, N22);
and AND2_3 (N27, N25, N1);
and AND2_4 (N28, N24, N23);
xor XOR2_2 (N29, N27, N28);

endmodule
