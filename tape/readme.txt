tapadd.pl
    Perl script for putting files into a tape image.  For example, to move
    a folder named "foo" from the host into the emulator, you might:
      (in the host)
      tar cvf foo.tar foo
      ./tapadd.pl foo.tar > foo.tap
      (at the emulator's command prompt)
      attach tq0 foo.tap
      (inside 211BSD)
      mt rewind
      tar xvf /dev/rmt12
  
tapcat.pl
    Perl script for extracting files from a tape image.  For example, to move
    a folder named "foo" from the emulator out to the host, you might:
      (at the emulator's command prompt)
      attach tq0 foo.tap
      (in the emulator)
      mt rewind
      tar cvf /dev/rmt12 foo
      (at the emulator's command prompt)
      detach ts
      (in the host)
      ./tapcat.pl foo.tap 0 > foo.tar
      tar xvf foo.tar

