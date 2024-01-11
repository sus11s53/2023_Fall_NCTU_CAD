//Verilog HDL for "ADC", "encoder" "functional"


module encoder ( In_0 , In_1 , In_2 , In_3 , In_4 , In_5 , In_6 , In_7 , In_8 , In_9 , In_10 , In_11 , In_12 , In_13 , In_14 , In_15 , Out_1 , Out_2 , Out_3 ,Out_4);
input In_0 , In_1 , In_2 , In_3 , In_4 , In_5 , In_6 , In_7 , In_8 , In_9 , In_10 , In_11 , In_12 , In_13 , In_14 , In_15;
output Out_1 ,Out_2 , Out_3 ,Out_4;

wire En;
wire [14:0] total_input;
reg [3:0] adc_out;


assign total_input = {In_15 , In_14 , In_13 , In_12 , In_11 , In_10 , In_9 , In_8 , In_7 , In_6 , In_5 , In_4 , In_3 , In_2 , In_1};
assign En = 1'b1;


always @(*) begin
    if(En == 1) begin
        case(total_input)
            15'b000000000000000 : adc_out = 4'b0000;
            15'b000000000000001 : adc_out = 4'b0001;
            15'b000000000000011 : adc_out = 4'b0010;
            15'b000000000000111 : adc_out = 4'b0011;
            15'b000000000001111 : adc_out = 4'b0100;
            15'b000000000011111 : adc_out = 4'b0101;
            15'b000000000111111 : adc_out = 4'b0110;
            15'b000000001111111 : adc_out = 4'b0111;
            15'b000000011111111 : adc_out = 4'b1000;
            15'b000000111111111 : adc_out = 4'b1001;
            15'b000001111111111 : adc_out = 4'b1010;
            15'b000011111111111 : adc_out = 4'b1011;
            15'b000111111111111 : adc_out = 4'b1100;
            15'b001111111111111 : adc_out = 4'b1101;
            15'b011111111111111 : adc_out = 4'b1110;
            15'b111111111111111 : adc_out = 4'b1111;
        endcase 
    end
    else adc_out = 0;
end

assign Out_1 = adc_out[0];
assign Out_2 = adc_out[1];
assign Out_3 = adc_out[2];
assign Out_4 = adc_out[3];

endmodule

