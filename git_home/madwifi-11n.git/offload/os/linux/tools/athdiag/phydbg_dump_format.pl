#use strict;
#use warnings;
use Getopt::Long;
use Time::HiRes;
use POSIX;
use Data::Dumper;
use constant ADDR_WIDTH => 13;
use constant TRACE_WIDTH => 40;

my $start = [ Time::HiRes::gettimeofday( ) ];
my $datetime2 = strftime( '%Y-%m-%d %H:%M:%S', localtime );
my %opts;
my $fpout;
GetOptions(\%opts, "i=s", "o=s");
die "Usage : $0 -i <input event_dump filename> -o <output filename>\n"
  if ((!exists $opts{'o'}) || (!exists $opts{'i'}));

my $trcNibs = (TRACE_WIDTH%4) ? int (TRACE_WIDTH/4) + 1 : TRACE_WIDTH/4;
open (IN, "<", $opts{'i'}) or die "Cannot open $opts{'i'}\n";
open ($fpout, ">", $opts{'o'}) or die "Cannot open $opts{'o'}\n";
my $chainid;
my $phydbg_mode;
my $j=0;
while (<IN>) {
  my $line = $_;
  chomp $line;
  if ($line =~ m/^chain_id: (\d)/){
	$chainid = $1;
	printf "chainid: $1 line: $j\n";
  }elsif ($line =~ m/^phydbg_mode: (.*)/){
	$phydbg_mode = $1;
	$phydbg_mode = hex $phydbg_mode;
	printf "phydbgmode: $1 line: $j\n";
	
  }
  next if ($line !~ m/\d: .*/);
  #$line =~ s/(\d):|W:)|(HIGH:)//g;
  my @ta = split(/[: \t]+/, $line);
 my $payload;
  $payload=sprintf( "%d", hex $ta[1]);

        my $samp_idx = 0;
        my $iq_idx   = 0;
		my $capdata;
        my ($rx_frame, $r_agc_sm, $r_srch_sm, $mb_gain, $rf_gain, $lower, $upper);
        my ($flag_adcsat, $flag_rfsat, $pwr_high, $pwr_low, $adc_q, $adc_i, $datq, $dati);

	#printf OUT "j: $j hex: $ta[1]\n";
	#printf $fpout "phydbg_mode: $phydbg_mode chain_id: $chainid j: $j hex: $ta[1]\n";
	#printf OUT "chain_id: $chainid j: $j hex: $ta[1]\n";
            #printf $fp ("%5d: 0x%x\n", $j, $payload);
			if(($phydbg_mode == 0) || ($phydbg_mode == 1)) {
        my $samp_idx = 0;
        my $iq_idx   = 0;
        my ($rx_frame, $r_agc_sm, $r_srch_sm, $mb_gain, $rf_gain, $lower, $upper);
        my ($flag_adcsat, $flag_rfsat, $pwr_high, $pwr_low, $adc_q, $adc_i, $datq, $dati);

     

            if($j%2 == 0) {
               $capdata = $payload;
            } else {
	       $lower       = $capdata;
	       $upper       = $payload; 

               $capdata = $capdata | ($payload << 32);
               $rx_frame    = ($capdata >> 63) & 0x1;
               $r_agc_sm    = ($capdata >> 60) & 0x7;
               $r_srch_sm   = ($capdata >> 56) & 0xf;
               $mb_gain     = ($capdata >> 40) & 0x7f;
               $rf_gain     = ($capdata >> 26) & 0xff;
               $flag_adcsat = ($capdata >> 25) & 0x1;
               $flag_rfsat  = ($capdata >> 24) & 0x1;
               $pwr_high    = ($capdata >> 23) & 0x1;
               $pwr_low     = ($capdata >> 22) & 0x1;
               $adc_q       = ($capdata >> 11) & 0x7ff;
               $adc_i       =  $capdata        & 0x7ff;


	       if($adc_q >= 1024)  {
                  $adc_q   = $adc_q - 2048; 
	       }
	       if($adc_i >= 1024)  {
                  $adc_i   = $adc_i - 2048; 
	       }

               printf $fpout ("%5d 0x%08x_%08x: rx\_frame=%x, r\_agc\_sm=0x%x, r\_srch\_sm=0x%x, mb\_gain%d[6:0]=%3d, rf\_gain%d[7:0]=%3d, flag\_adcsat_any=%x, flag\_rfsat_any=%x, pwr\_high%d=%x, pwr\_low%d=%x, rx\_pri80\_q%d[11:1]=%3d, rx\_pri80\_i%d[11:1]=%3d\n", 

                               $samp_idx, $upper, $lower, $rx_frame, $r_agc_sm, $r_srch_sm, 
                               $chainid, $mb_gain, $chainid, $rf_gain, 
                               $flag_adcsat, $flag_rfsat, 
                               $chainid, $pwr_high, $chainid, $pwr_low, 
                               $chainid, $adc_q, $chainid, $adc_i);
               $samp_idx++;
               $capdata = 0;
            }
        
    #---------------------------------------------------------------
    # MODE 2  (Rx I/Q at 80 Msps -- after decimation filter)
    #---------------------------------------------------------------
    } elsif($phydbg_mode == 2) {
        my $samp_idx = 0;
        my $iq_idx   = 0;
        my ($rx_frame, $r_agc_sm, $r_srch_sm, $w0, $w1, $w2, $w3);
        my ($mb_gain3, $mb_gain2, $mb_gain1, $mb_gain0, $rf_gain3, $rf_gain2, $rf_gain1, $rf_gain0);
        my ($adcsat3, $adcsat2, $adcsat1, $adcsat0, $rfsat3, $rfsat2, $rfsat1, $rfsat0);
        my ($pwr_high3, $pwr_high2, $pwr_high1, $pwr_high0, $pwr_low3, $pwr_low2, $pwr_low1, $pwr_low0);
        my ($adc_q3, $adc_q2, $adc_q1, $adc_q0, $adc_i3, $adc_i2, $adc_i1, $adc_i0);

        
            if($j%4 == 0) {
                $capdata = $payload;
                $w0      = $payload;
            } elsif($j%4 == 1) {
                $capdata = $capdata | ($payload << 32);
                $w1      = $payload;
            } elsif($j%4 == 2) {
                $capdata = $capdata | ($payload << 64);
                $w2      = $payload;
            } else {
                $capdata = $capdata | ($payload << 96);
                $w3      = $payload;

                $rx_frame    = ($capdata >> 127) & 0x1;
                $r_agc_sm    = ($capdata >> 124) & 0x7;
                $r_srch_sm   = ($capdata >> 120) & 0xf;

                $mb_gain3    = ($capdata >> 113) & 0x7f;
                $mb_gain2    = ($capdata >> 106) & 0x7f;
                $mb_gain1    = ($capdata >>  99) & 0x7f;
                $mb_gain0    = ($capdata >>  92) & 0x7f;
                $rf_gain3    = ($capdata >>  85) & 0x7f;
                $rf_gain2    = ($capdata >>  78) & 0x7f;
                $rf_gain1    = ($capdata >>  71) & 0x7f;
                $rf_gain0    = ($capdata >>  64) & 0x7f;

                $adcsat3     = ($capdata >>  63) & 0x1;
                $adcsat2     = ($capdata >>  62) & 0x1;
                $adcsat1     = ($capdata >>  61) & 0x1;
                $adcsat0     = ($capdata >>  60) & 0x1;
                $rfsat3      = ($capdata >>  59) & 0x1;
                $rfsat2      = ($capdata >>  58) & 0x1;
                $rfsat1      = ($capdata >>  57) & 0x1;
                $rfsat0      = ($capdata >>  56) & 0x1;

                $pwr_high3   = ($capdata >>  55) & 0x1;
                $pwr_high2   = ($capdata >>  54) & 0x1;
                $pwr_high1   = ($capdata >>  53) & 0x1;
                $pwr_high0   = ($capdata >>  52) & 0x1;
                $pwr_low3    = ($capdata >>  51) & 0x1;
                $pwr_low2    = ($capdata >>  50) & 0x1;
                $pwr_low1    = ($capdata >>  49) & 0x1;
                $pwr_low0    = ($capdata >>  48) & 0x1;

                $adc_q3      = ($capdata >>  42) & 0x3f;
                $adc_q2      = ($capdata >>  36) & 0x3f;
                $adc_q1      = ($capdata >>  30) & 0x3f;
                $adc_q0      = ($capdata >>  24) & 0x3f;
                $adc_i3      = ($capdata >>  18) & 0x3f;
                $adc_i2      = ($capdata >>  12) & 0x3f;
                $adc_i1      = ($capdata >>   6) & 0x3f;
                $adc_i0      =  $capdata         & 0x3f;

	        if($adc_q0 >= 32)  { $adc_q0   = $adc_q0 - 64; }
	        if($adc_q1 >= 32)  { $adc_q1   = $adc_q1 - 64; }
	        if($adc_q2 >= 32)  { $adc_q2   = $adc_q2 - 64; }
	        if($adc_q3 >= 32)  { $adc_q3   = $adc_q3 - 64; }

	        if($adc_i0 >= 32)  { $adc_i0   = $adc_i0 - 64; }
	        if($adc_i1 >= 32)  { $adc_i1   = $adc_i1 - 64; }
	        if($adc_i2 >= 32)  { $adc_i2   = $adc_i2 - 64; }
	        if($adc_i3 >= 32)  { $adc_i3   = $adc_i3 - 64; }

                printf $fpout ("%5d 0x%08x_%08x_%08x_%08x: rx\_frame=%x, r\_agc\_sm=0x%x, r\_srch\_sm=0x%x, mb\_gain[6:0]=%3d %3d %3d %3d, rf\_gain[7:1]=%3d %3d %3d %3d, flag\_adcsat=%x_%x_%x_%x, flag\_rfsat=%x_%x_%x_%x, pwr\_high=%x_%x_%x_%x, pwr\_low=%x_%x_%x_%x, rx\_pri40\_q[11:6]=%3d %3d %3d %3d, rx\_pri40\_i[11:6]=%3d %3d %3d %3d\n", 

                                $samp_idx, $w3, $w2, $w1, $w0, $rx_frame, $r_agc_sm, $r_srch_sm, 
                                $mb_gain3, $mb_gain2, $mb_gain1, $mb_gain0, 
                                $rf_gain3, $rf_gain2, $rf_gain1, $rf_gain0, 
                                $adcsat3, $adcsat2, $adcsat1, $adcsat0, 
                                $rfsat3, $rfsat2, $rfsat1, $rfsat0, 
                                $pwr_high3, $pwr_high2, $pwr_high1, $pwr_high0, 
                                $pwr_low3, $pwr_low2, $pwr_low1, $pwr_low0, 
                                $adc_q3, $adc_q2, $adc_q1, $adc_q0, 
                                $adc_i3, $adc_i2, $adc_i1, $adc_i0);
                $samp_idx++;
                $capdata = 0;       
            }
        
    #---------------------------------------------------------------
    # MODE 3  (Rx I/Q at 80 Msps -- after decimation filter)
    #---------------------------------------------------------------
    } elsif($phydbg_mode == 3) {
        my $samp_idx = 0;
        my ($rx_frame, $r_agc_sm, $r_srch_sm, $flag_adcsat, $flag_rfsat, $w0, $w1, $w2, $w3);
        my ($mb_gain3, $mb_gain2, $mb_gain1, $mb_gain0, $rf_gain3, $rf_gain2, $rf_gain1, $rf_gain0);
        my ($pwr_high3, $pwr_high2, $pwr_high1, $pwr_high0, $pwr_low3, $pwr_low2, $pwr_low1, $pwr_low0);
        my ($adc_q3, $adc_q2, $adc_q1, $adc_q0, $adc_i3, $adc_i2, $adc_i1, $adc_i0);

           if($j%4 == 0) {
              $capdata = $payload;
              $w0      = $payload;
           } elsif($j%4 == 1) {
              $capdata = $capdata | ($payload << 32);
              $w1      = $payload;
           } elsif($j%4 == 2) {
              $capdata = $capdata | ($payload << 64);
              $w2      = $payload;
           } else {
              $capdata = $capdata | ($payload << 96);
              $w3      = $payload;

              $rx_frame    = ($capdata >> 127) & 0x1;
              $r_agc_sm    = ($capdata >> 124) & 0x7;
              $r_srch_sm   = ($capdata >> 120) & 0xf;

              $mb_gain3    = ($capdata >> 116) & 0xf;
              $mb_gain2    = ($capdata >> 112) & 0xf;
              $mb_gain1    = ($capdata >> 108) & 0xf;
              $mb_gain0    = ($capdata >> 104) & 0xf;

              $rf_gain3    = ($capdata >> 101) & 0x7; # 3 bits
              $rf_gain2    = ($capdata >>  98) & 0x7; # 3 bits
              $rf_gain1    = ($capdata >>  94) & 0xf; # 4 bits
              $rf_gain0    = ($capdata >>  90) & 0xf; # 4 bits

              $flag_adcsat = ($capdata >>  89) & 0x1;
              $flag_rfsat  = ($capdata >>  88) & 0x1;

              $pwr_high3   = ($capdata >>  87) & 0x1;
              $pwr_high2   = ($capdata >>  86) & 0x1;
              $pwr_high1   = ($capdata >>  85) & 0x1;
              $pwr_high0   = ($capdata >>  84) & 0x1;
              $pwr_low3    = ($capdata >>  83) & 0x1;
              $pwr_low2    = ($capdata >>  82) & 0x1;
              $pwr_low1    = ($capdata >>  81) & 0x1;
              $pwr_low0    = ($capdata >>  80) & 0x1;

              $adc_q3      = ($capdata >>  70) & 0x3ff;
              $adc_q2      = ($capdata >>  60) & 0x3ff;
              $adc_q1      = ($capdata >>  50) & 0x3ff;
              $adc_q0      = ($capdata >>  40) & 0x3ff;
              $adc_i3      = ($capdata >>  30) & 0x3ff;
              $adc_i2      = ($capdata >>  20) & 0x3ff;
              $adc_i1      = ($capdata >>  10) & 0x3ff;
              $adc_i0      =  $capdata         & 0x3ff;

			  if($adc_q0 >= 512)  { $adc_q0   = $adc_q0 - 1024; }
			  if($adc_q1 >= 512)  { $adc_q1   = $adc_q1 - 1024; }
			  if($adc_q2 >= 512)  { $adc_q2   = $adc_q2 - 1024; }
			  if($adc_q3 >= 512)  { $adc_q3   = $adc_q3 - 1024; }

			  if($adc_i0 >= 512)  { $adc_i0   = $adc_i0 - 1024; }
			  if($adc_i1 >= 512)  { $adc_i1   = $adc_i1 - 1024; }
			  if($adc_i2 >= 512)  { $adc_i2   = $adc_i2 - 1024; }
			  if($adc_i3 >= 512)  { $adc_i3   = $adc_i3 - 1024; }

				  printf $fpout ("%4d 0x%08x_%08x_%08x_%08x: rx\_frame=%x, r\_agc\_sm=0x%x, r\_srch\_sm=0x%x, mb\_gain[6:3]=%2d %2d %2d %2d, rf\_gain[7:4]=%d %d %d %d, flag\_adcsat_any=%x, flag\_rfsat_any=0x%x, pwr\_high =%x_%x_%x_%x, pwr\_low =%x_%x_%x_%x, rx\_pri40\_q[11:2]=%3d %3d %3d %3d, rx\_pri40\_i[11:2]=%3d %3d %3d %3d\n", 

								 $samp_idx, $w3, $w2, $w1, $w0, $rx_frame, $r_agc_sm, $r_srch_sm, 
								 $mb_gain3, $mb_gain2, $mb_gain1, $mb_gain0, 
								 $rf_gain3, $rf_gain2, $rf_gain1, $rf_gain0, 
								 $flag_adcsat, $flag_rfsat, 
								 $pwr_high3, $pwr_high2, $pwr_high1, $pwr_high0, 
								 $pwr_low3, $pwr_low2, $pwr_low1, $pwr_low0, 
								 $adc_q3, $adc_q2, $adc_q1, $adc_q0, 
								 $adc_i3, $adc_i2, $adc_i1, $adc_i0);
				  $samp_idx++;
				  $capdata = 0;          
            }

    } elsif($phydbg_mode == 5) {
		#printf "inside dbgmode 5\n";
        my ($rx_frame, $mb_gain, $flag_adcsat, $flag_rfsat, $r_srch_sm, $adc_q, $adc_i);

           $rx_frame    = ($payload >> 31) & 0x1;
           $mb_gain     = ($payload >> 26) & 0x1f;
           $flag_adcsat = ($payload >> 25) & 0x1;
           $flag_rfsat  = ($payload >> 24) & 0x1;
           $r_srch_sm   = ($payload >> 20) & 0xf;
           $adc_q       = ($payload >> 10) & 0x3ff;
           $adc_i       = $payload & 0x3ff;  

		   if($adc_q >= 512)  {
				  $adc_q   = $adc_q - 1024; 
		   }
		   if($adc_i >= 512)  {
				  $adc_i   = $adc_i - 1024; 
		   }


			   printf $fpout ("%5d 0x%08x: rx\_frame=%x, mb\_gain%d[6:2]=%2d, flag\_adcsat%d=%x, flag\_rfsat%d=%x, r\_srch\_sm=0x%x, adc\_fifo\_q%d[9:0]=%3d, adc\_fifo\_i%d[9:0]=%3d\n", 

							   $j, $payload, $rx_frame, $chainid, $mb_gain, $chainid, $flag_adcsat, $chainid, $flag_rfsat, $r_srch_sm, 
							   $chainid, $adc_q, $chainid, $adc_i);

    }elsif($phydbg_mode == 7){
		  my $is_rx_tlv,$tlv_start,$tlv_data;
		  $is_rx_tlv = ($payload >> 31) & 0x1; 
		  $tlv_start = ($payload >> 30) & 0x1; 
		  $tlv_data  = $payload & 0x3fffffff;        
		  if($tlv_start)
		  {
			printf $fpout ("\n");
		  }
		  printf $fpout ("%5d: is\_rx\_tlv=%d, tlv\_start=%d, tlv\_data=0x%x\n", $j, $is_rx_tlv, $tlv_start, $tlv_data);
	}
	$j++;
	}

