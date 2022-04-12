class GrammarConverter
	def initialize
		@grammar = ""
		@filename = ""
		@regex_angle_brackets = /(?<!')(\b[a-z_]{2,})/
		if ARGV.length == 1
			@filename = ARGV[0]
			puts "Convertendo o arquivo " << @filename
			readFile()
			convert()
		else
			puts "Insira o nome do arquivo"
			exit(1)
		end
    end

	def readFile()
		file = File.open(@filename, 'r')
		lines = file.readlines.map(&:chomp)
		file.close()
		lines.each do |line|
			@grammar << line << 10
		end
	end

	def converted_file_name()
		components = @filename.split(".")
		return components[0] << "_converted." << components[1]
	end

    def convert()
    	puts @grammar
    	target = File.open('./' << converted_file_name() , 'w')
        @grammar = @grammar.gsub(@regex_angle_brackets, '<\1>')
        @grammar = @grammar.gsub(/\"/, '\\\"')
        @grammar = @grammar.gsub(/\'/, '"')
        @grammar = @grammar.gsub(/(?<!\|)\|(?!\|)/, ' | ')
        puts @grammar
		target.write(@grammar)
		target.close()
        puts "Conversão salva em " << converted_file_name()
    end
end


tg = GrammarConverter.new
