#!/usr/bin/env perl
use FindBin;
use lib "$FindBin::Bin";
use strict;
use metadata;
use Getopt::Std;
use Excel::Writer::XLSX;

use Data::Dumper;

my %opts;
my %localpkgs;

$Getopt::Std::STANDARD_HELP_VERSION = 1;

sub HELP_MESSAGE {
	print "$ARGV[0] [-o file.xlsx]\n";
}

sub get_info_from_make($$) {
	my $ret=shift;
	my $dir=shift;

	my @varlist = (	"var.PKG_NAME",
			"var.PKG_SOURCE",
			"var.PKG_VERSION",
			"var.LINUX_SOURCE",
			"var.LINUX_VERSION",
	);
	my $varstr = join(" ", @varlist);

	my @pkgname = `make -C ${dir} ${varstr} --no-print-directory TOPDIR=\`pwd\` 2>/dev/null`;
	chomp @pkgname;

	return if ($#pkgname==-1);

	foreach (@pkgname) {
		(my $key, my $value) = ($_ =~ m/(.*)=\'(.*)\'/);
		$ret->{$key}=$value;
	}
}

sub get_host_tools() {
	my @hosttools;

	printf("Get host tools list...");
	@hosttools = split / /,`make --no-print-directory val.tools-y V=s 2>/dev/null`;
	printf("done\n");

	chomp @hosttools;
	@hosttools = grep { ! /^$/ } @hosttools;

	foreach my $pkg (sort @hosttools) {
		my %h_pkg;

		printf("\033[M\r");
		printf("Get host src pkg name...$pkg");
		get_info_from_make(\%h_pkg, "tools/$pkg");

		if (-f "dl/$h_pkg{PKG_SOURCE}") {
			$localpkgs{$h_pkg{PKG_NAME}}->{host} = "x";
			$localpkgs{$h_pkg{PKG_NAME}}->{version} = $h_pkg{PKG_VERSION};
			$localpkgs{$h_pkg{PKG_NAME}}->{source} = $h_pkg{PKG_SOURCE};
		}
	}
	printf("\033[M\r");
	printf("Get host src pkg name...done\n");
}

sub get_target_pkgs() {
	printf("Get target pkg name...");
	parse_package_metadata("tmp/.packageinfo") or exit 1;

	foreach my $pkg (sort {uc($a) cmp uc($b)} keys %package) {
		my $pkgname = $package{$pkg}->{src};
		printf("\033[M\r");
		printf("Get target pkg name...$pkg");
		if (-f "dl/$package{$pkg}->{source}") {
			$localpkgs{$pkgname}->{target} = "x";
			$localpkgs{$pkgname}->{version} = $package{$pkg}->{version};
			$localpkgs{$pkgname}->{source} = $package{$pkg}->{source};

			# Subdir field contains the full dir (feeds/package/)
			# We just want the feed name (package)
			if (! $package{$pkg}->{subdir} =~ m/^$/) {
				$package{$pkg}->{subdir} =~ s,feeds/(.*)/,$1,;
				$localpkgs{$pkgname}->{feed} = $package{$pkg}->{subdir}
			}
		}
	}

	printf("\033[M\r");
	printf("Get target pkg name...done\n");
}

sub get_toolchain_pkgs() {
	my @toolchaindirs = (
			"binutils",
			"eglibc",
			"gcc",
			"gdb",
			"kernel-headers",
			"llvm",
			"uClibc",
			"wrapper",
		);
	foreach my $pkg (sort @toolchaindirs) {
		my %h_pkg;

		printf("\033[M\r");
		printf("Get toolchain pkg name...");
		get_info_from_make(\%h_pkg, "toolchain/$pkg");

		if (-f "dl/$h_pkg{PKG_SOURCE}") {
			$localpkgs{$h_pkg{PKG_NAME}}->{toolchain} = "x";
			$localpkgs{$h_pkg{PKG_NAME}}->{version} = $h_pkg{PKG_VERSION};
			$localpkgs{$h_pkg{PKG_NAME}}->{source} = $h_pkg{PKG_SOURCE};
		}
	}
	printf("\033[M\r");
	printf("Get toolchain pkg name...done\n");
}

sub get_linux_pkg() {
	my $targetdir = "target/linux";

	opendir(DIR, $targetdir) or die("\"$targetdir\" does not exist");
	foreach my $target (readdir(DIR)) {
		my %h_pkg;

		next if ! -d "$targetdir/$target";
		printf("\033[M\r");
		printf("Get kernel pkg name...$target");
		get_info_from_make(\%h_pkg, "$targetdir/$target");
		if (-f "dl/$h_pkg{LINUX_SOURCE}") {
			$localpkgs{"linux"}->{target} = "x";
			$localpkgs{"linux"}->{version} = $h_pkg{LINUX_VERSION};
			$localpkgs{"linux"}->{source} = $h_pkg{LINUX_SOURCE};
		}
	}
	printf("\033[M\r");
	printf("Get kernel pkg name...done\n");
	closedir(DIR);
}

sub is_downloaded($) {
	my $source = shift;
	-f "dl/$source" and return 1;
	return 0;
}

sub write_txt() {
	printf("%-30s\t%-20s\t%-70s\t%-8s\t%-12s\t%-12s\t%-20s\n",
		"name", "feed", "source","host","toolchain","target","version");
	foreach my $src (sort keys %localpkgs) {
		# We fill in the hash with empty strings to facilitate the print below
		$localpkgs{$src}->{host} = "" unless exists($localpkgs{$src}->{host});
		$localpkgs{$src}->{toolchain} = "" unless exists($localpkgs{$src}->{toolchain});
		$localpkgs{$src}->{target} = "" unless exists($localpkgs{$src}->{target});
		printf("%-30s\t%-20s\t%-70s\t%-8s\t%-12s\t%-12s\t%-20s\n",
			$src,
			$localpkgs{$src}->{source},
			$localpkgs{$src}->{feed},
			$localpkgs{$src}->{host},
			$localpkgs{$src}->{toolchain},
			$localpkgs{$src}->{target},
			$localpkgs{$src}->{version});
	}
}

sub write_xlsx() {
	my $workbook = Excel::Writer::XLSX->new($opts{o});
	if (!defined($workbook)) {
		die "Error: Unable to open \"$opts{o}\"";
	}

	my $worksheet = $workbook->add_worksheet();
	$worksheet->set_column("A:A", 30);
	$worksheet->set_column("B:B", 50);
	$worksheet->set_column("C:C", 30);
	$worksheet->set_column("D:D", 15);
	$worksheet->autofilter("D:G");

	my $title_format = $workbook->add_format();
	$title_format->set_bold();
	$title_format->set_align("center");

	my $select_format = $workbook->add_format();
	$select_format->set_align("center");

	my $row = 0;
	$worksheet->write($row, 0, "name", $title_format);
	$worksheet->write($row, 1, "source", $title_format);
	$worksheet->write($row, 2, "version", $title_format);
	$worksheet->write($row, 3, "feed", $title_format);
	$worksheet->write($row, 4, "host", $title_format);
	$worksheet->write($row, 5, "toolchain", $title_format);
	$worksheet->write($row, 6, "target", $title_format);

	foreach my $src (sort keys %localpkgs) {
		$row++;
		$worksheet->write($row, 0, $src);
		$worksheet->write($row, 1, $localpkgs{$src}->{source});
		$worksheet->write_string($row, 2, $localpkgs{$src}->{version})
				if exists($localpkgs{$src}->{version});
		$worksheet->write($row, 3, $localpkgs{$src}->{feed})
				if exists($localpkgs{$src}->{feed});
		$worksheet->write($row, 4, $localpkgs{$src}->{host}, $select_format)
				if exists($localpkgs{$src}->{host});
		$worksheet->write($row, 5, $localpkgs{$src}->{toolchain}, $select_format)
				if exists($localpkgs{$src}->{toolchain});
		$worksheet->write($row, 6, $localpkgs{$src}->{target}, $select_format)
				if exists($localpkgs{$src}->{target});
	}
}

sub main() {
	get_host_tools();
	get_target_pkgs();
	get_toolchain_pkgs();
	get_linux_pkg();

	write_txt();
	write_xlsx() if (exists($opts{o}) && defined($opts{o}));
}

getopts('o:', \%opts);
main();
