use strict;
use warnings;
use Getopt::Long;
use Time::HiRes;
use POSIX;
use Data::Dumper;
use constant ADDR_WIDTH => 13;
use constant TRACE_WIDTH => 40;

my $start = [ Time::HiRes::gettimeofday( ) ];
my $datetime2 = strftime( '%Y-%m-%d %H:%M:%S', localtime );
my %opts;
GetOptions(\%opts, "i=s", "o=s");
die "Usage : $0 -i <input event_dump filename> -o <output filename>\n"
  if ((!exists $opts{'o'}) || (!exists $opts{'i'}));

my $trcNibs = (TRACE_WIDTH%4) ? int (TRACE_WIDTH/4) + 1 : TRACE_WIDTH/4;
open (IN, "<", $opts{'i'}) or die "Cannot open $opts{'i'}\n";
open (OUT, ">", $opts{'o'}) or die "Cannot open $opts{'o'}\n";


my $file="./beeliner_mac_events.h";
my $out_file="./eventID.txt";
my %dataHash;
my $dataComponentShiftName;
my $dataComponentShiftValue;
my $dataComponentMaskName;
my $dataComponentMaskValue;
my $dataComponent;
my $eventName;
my $moduleName;
my $moduleEvent;
open (HDR,"<$file") or die "Can't open $file: $!\n";
#open (OUTHDR,">$out_file") or die "Can't open $out_file: $!\n";
#open (PopulateEventIdInfoArray, ">", "eventIdInfoArray.txt") or die "Cannot open eventIdInfoArray.txt\n";
#open (PopulateEventIdHdr, ">", "beelinerEventId.h") or die "Cannot open beelinerEventId.h\n";
#open (PopulateEventSelect, ">", "eventSelect.txt") or die "Cannot open eventSelect.txt\n";
#open (PopulateEventIdInfo, ">", "eventIdInfoHash.txt") or die "Cannot open eventId.txt\n";

my $cnt=0;

	#printf PopulateEventIdInfoArray "A_UINT32 eventIdInfo[] = { \n";

my %moduleHash;
	
while (<HDR>){
my $shiftMaskHash={};
 

if(/^\/\/ Module ID : (\w+).*Module : (\w+).*Event ID : (\w+).*Event : (\w+)/){
	my $moduleID_hdr = $1;
	my $moduleName_hdr = $2;
	my $eventID_hdr = $3;
	my $eventName_hdr = $4;
	$moduleHash{$moduleID_hdr}{$eventID_hdr} = $moduleName_hdr."_".$eventName_hdr."_EVENT";
	#printf PopulateEventIdInfoArray "%s,\n",$moduleName_hdr."_".$eventName_hdr."_EVENT_ID";
	#printf PopulateEventSelect "%-5d%-60s0\n",$cnt,$moduleHash{$moduleID_hdr}{$eventID_hdr};
	$eventName=$moduleName_hdr."_".$eventName_hdr;
	$moduleEvent=$moduleName_hdr."_".$eventName_hdr."_";
	#printf "$1\n";
	
	#printf OUTHDR "$moduleEvent\n";
	$cnt++;
}

	if(/^\s*#define\s*(\S+_LSB)\s+(.+)$/){
	$dataComponentShiftName=$1;
	$dataComponentShiftValue=$2;
	$dataComponentShiftName =~ s/$moduleEvent//;
	$dataComponent = $dataComponentShiftName;
	$dataComponent =~ s/_LSB$//;
	
	#printf OUTHDR "$dataComponentShiftName\t$dataComponentShiftValue\n";
	}
	
	if(/^\s*#define\s*(\S+_MASK)\s+(.+)$/){
	$dataComponentMaskName=$1;
	$dataComponentMaskValue=$2;
	$dataComponentMaskName =~ s/$moduleEvent//;
	$dataComponentMaskName =~ s/_MASK$//;
	$dataHash{$eventName}{$dataComponent}=$shiftMaskHash;
	$shiftMaskHash->{"shift"}=$dataComponentShiftValue;
	$shiftMaskHash->{"mask"}=$dataComponentMaskValue;
	
	#printf OUTHDR "$dataComponentMaskName\t$dataComponentMaskValue\n";
	
	}
	
	if(/^\s*(#define\s*\S+_EVENT_ID\s+.+)$/){
		#printf PopulateEventIdHdr "$1\n";
	}
	if(/^\s*#define\s*(\S+_EVENT_ID)\s+(.+)$/){
		my $event_id_info_name = $1;
		my $event_id_info_id = $2;
		$event_id_info_name =~ s/_EVENT_ID/_EVENT/;
		#printf "$event_id_info_name\n";
		#$event_id_info_hash_value =~ s/_EVENT_ID/_EVENT/;
		#printf PopulateEventIdInfo "$event_id_info_id => \'$event_id_info_name\',\n";
	}
}
#printf Dumper(\%moduleHash);
#printf PopulateEventIdInfoArray "}; \n";
#printf OUTHDR Dumper(\%dataHash);

close HDR;
#close PopulateEventIdHdr;
#close PopulateEventSelect;
#close PopulateEventIdInfoArray;
#close PopulateEventIdInfo;

my $ts = 0;
my $tp = 10;
my $addrWid = ADDR_WIDTH;
my $highValue32;
my $lowValue32;
my $tsMultiplier;
my $timeStamp;
my $traceCode;
my $eventID;
my $moduleID;
my $data;
my $tsMultiplierMask32=0x000000C0;
my $timeStampMask32=0x0000003F;
my $traceCodeMask32=0xC0000000;
my $moduleMask32=0x3c000000;
my $eventMask32=0x03F00000;
my $dataMask32=0x000FFFFF;

my $TIMESTAMP_MULT_BITS_SHIFT=6;
my $TRACECODE_BITS_SHIFT=30;
my $MODULE_BITS_SHIFT=26;
my $EVENT_BITS_SHIFT=20;


my $tsMultiplierWid=2;
my $timeStampWid=6;
my $traceCodeWid=2;
my $moduleWid=4;
my $eventWid=6;
my $dataWid=20;

use constant DELTA_TIME0 => 1;
use constant DELTA_TIME1 => 32;
use constant DELTA_TIME2 => 2048;
use constant DELTA_TIME3 => 131072;

my @timeStampLut = (DELTA_TIME0,DELTA_TIME1,DELTA_TIME2,DELTA_TIME3);

my $multiplierClocks;
my $ClksBwEvents;
my $cumulativeClocks=0;
my $clkMhz = 150;
my $clockPeriod=1/$clkMhz; #Assuming clock is 150MHz
my $cumulativeTime=0;
my $incrementalClocks=0;
my $totalTime=0;
my $cumClks=0;

my $SNum=0;
my $j=0;
printf OUT ("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
printf OUT ("%-5s\t%-27s\t%-58s\t%-8s\t%-40s\n","#", "CumulativeTS in us(clks)", "EventName", "Data","DataDetails");
printf OUT ("--------------------------------------------------------------------------------------------------------------------------------------------------------\n");
my $dataHash={};
sub decodeData
{
my $file=shift; #removes and returns the 1st element of the array and truncates it by 1 element. So, it's the output file handler

my $data2=shift;
my $moduleId_hex2 = shift;
my $eventId_hex2 = shift;

#my $moduleID2=shift;
#my $eventID2=shift;
my $eventName;
my $value;

#$eventName = $moduleEventNames{$moduleID}{$eventID};
#$eventName = $moduleHash{$moduleID}{$eventID};
#printf "$moduleHash{'$moduleID'}{'$eventID'}\n";
#my $moduleId_hex2 = sprintf("%#x",$moduleID);
#my $eventId_hex2 = sprintf("%#x",$eventID);

#printf "$moduleHash{$moduleId_hex}{$eventId_hex}\n";
$eventName = $moduleHash{$moduleId_hex2}{$eventId_hex2};
#printf "$moduleHash{'0x3'}{'0xe'}\n";
#printf "$moduleID    $eventID %x   %x m$moduleId_hex: e$eventId_hex:\n",$moduleID,$eventID;
	if (defined $eventName && $eventName ne ""){
	
		$eventName =~ s/_EVENT$//;
		my $keyCount=0;
		my $spaceCount=0;
		foreach my $dataComp (keys %{$dataHash{$eventName}}){

		my $value2=($data2 & hex $dataHash{$eventName}{$dataComp}{mask})>>$dataHash{$eventName}{$dataComp}{shift};
		if ($keyCount==0){$spaceCount=0;}else{$spaceCount=120;}
		printf $file "%*s%-40s %-#5x\n",$spaceCount,"",$dataComp,$value2;#-------GOOOOOOOOOOOOOOOOOOOOD
		$keyCount+=1;
		}
	}else{
	printf $file "\n";
	}
$j+=1;
}

while (<IN>) {
  my $line = $_;
  chomp $line;
  next if ($line !~ m/TRC: .*/);
  $line =~ s/(TRC:)|(LOW:)|(HIGH:)//g;
  my @ta = split(/[ \t]+/, $line);

  $highValue32=sprintf( "%d", hex $ta[3] );
  $lowValue32=sprintf( "%d", hex $ta[2]  );
  
   $ta[2] = $ta[3] . $ta[2];
  
  #decoding timestamp multiplier from high value
  $tsMultiplier=($highValue32 & $tsMultiplierMask32)>>$TIMESTAMP_MULT_BITS_SHIFT;
  #decoding timestamp from high value
  $timeStamp=($highValue32 & $timeStampMask32);
  #decoding tracecode from low value
  $traceCode=($lowValue32 & $traceCodeMask32)>>$TRACECODE_BITS_SHIFT;
  #decoding eventID from low value
  $eventID=($lowValue32 & $eventMask32)>>$EVENT_BITS_SHIFT;
  #decoding moduleID from low value
  $moduleID=($lowValue32 & $moduleMask32)>>$MODULE_BITS_SHIFT;
  #decoding data from low value

  $data=($lowValue32 & $dataMask32);
  $multiplierClocks=$timeStampLut[$tsMultiplier];
  $ClksBwEvents=$multiplierClocks*$timeStamp;
  $cumClks+=$ClksBwEvents if $SNum !=0;
  $totalTime = $cumClks*$clockPeriod;
  printf OUT ("%-5d\t",$SNum);

  if($incrementalClocks+$ClksBwEvents>$clkMhz){
  $cumulativeClocks=$cumulativeClocks+$ClksBwEvents+$incrementalClocks if $SNum !=0;#cumClocks start from zero
  $cumulativeTime=$cumulativeClocks*$clockPeriod;
  printf OUT ("%-27.0f\t",$cumulativeTime);
  $incrementalClocks=0;
  }else{
  $incrementalClocks+=$ClksBwEvents;
  printf OUT ("%-10.0f%-5s%10s\t",$cumulativeTime,"($incrementalClocks)"," ");
  }
  #Converting moduleID and eventID to hex
  my $moduleId_hex = sprintf("%#x",$moduleID);
  my $eventId_hex = sprintf("%#x",$eventID);
    
  if (defined $moduleHash{$moduleId_hex}{$eventId_hex} && $moduleHash{$moduleId_hex}{$eventId_hex} ne ""){
	printf OUT ("%-58s\t",$moduleHash{$moduleId_hex}{$eventId_hex});
  }else{
	printf OUT ("%-58s\t","INVALID DATA");
  }
  printf OUT ("%-#10x\t",$data);

  decodeData(\*OUT, $data, $moduleId_hex, $eventId_hex);
  
  $ts += $tp;
  $SNum+=1;
}

#open (moduleHash, ">", "moduleHash.txt") or die "Cannot open moduleHash.txt\n";
#open (moduleEventNames, ">", "moduleEventNames.txt") or die "Cannot open moduleEventNames.txt\n";
#printf moduleHash Dumper(\%moduleHash);
#printf moduleHash "%60s",$moduleHash{'0x3'}{'0xe'};
#printf moduleEventNames Dumper(\%moduleEventNames);
#close moduleHash;
#close moduleEventNames;

close IN;
close OUT;
