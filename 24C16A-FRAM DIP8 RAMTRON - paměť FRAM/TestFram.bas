#PICAXE 20M2
#no_data
#terminal 19200

;Test zápisu a čtení paměti AT24C16A - FRAM

symbol MEM_ADR_WRITE = 104
symbol MEM_ADR_READ = 2


SETFREQ M16

gosub subTest2
;gosub subTest1

;Přečte 32 bytů, přidá k nim jedničku a opět je zapíše
;testujeme, zda se dají zapsat data do dvou bloků jedním příkazem 
;Blok je u této paměti dlouhý 16 bytů
subTest2:
   
   bptr = MEM_ADR_WRITE

   ;počáteční stav
   for b0=1 to 16
      @bptrInc = b0
   next

   ;inicializujeme I2C, stránka 0
   ;b - block address
   ;                         bbbx
   Hi2CSetup i2cmaster, %10100000, i2cfast_16, i2cbyte

  
   ;čteme z RAM a zapisujeme do FRAM
   bptr = MEM_ADR_WRITE

   HI2cout 0,( _
        @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      , @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      , @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      , @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      )
   
   ;čteme z FRAM a zapíšeme do RAM
   bptr = MEM_ADR_READ

   HI2cIN 0,( _
        @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      , @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      , @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      , @bptrInc, @bptrInc, @bptrInc, @bptrInc _
      )
   
   debug   
      
   ;vypneme I2C
   Hi2CSetup off
      
return




;Přečte 2 byty, přidá k nim jedničku a opět je zapíše
;a tak furt dokola...la...la
subTest1:
   ;b - block address
   ;                         bbbx
   Hi2CSetup i2cmaster, %10100000, i2cfast_16, i2cbyte
   
   do
      ;čteme
      HI2cin 0,(b0, b1)
   
      sertxd ("b0=", #b0, "; b1=", #b1, CR)
   
      ;inkrementujeme
      inc b0
      inc b1
   
      ;zapisujeme
      hi2cout 0,(b0, b1)   
   
      ;čekáme
      pause 1000
      
   loop
return

