# Note
  - 这是一个用于嵌入式系统的简易的配置库。用来保存，读写在flash中的配置。
  - 对flash做了抽象，适用于linux系统和其它嵌入式系统。使用时，根据平台做一些非常简单的适配工作，适配代码在hal目录。
  - 这个配置库，方便移植，使用也非常简单。可以批量修改后再一起保存配置并写入flash，减少flash的擦写次数。
  - 对flash空间的利用非常充分，几乎不会因为配置数量的增加，而浪费大量的flash空间。在flash中的存储格式也非常简单，跟普通的配置文件格式类似，因此可以支持带有文件系统的平台。
  - 支持多种类型的配置，包括字符串，字符数组，unsigned int，unsigned short，unsigned char。
  - 舍弃传统的用一个大的数据结构来保存配置，传统的方法在可扩展性，可维护性方面表现都非常差，而且耦合性也非常强。使用这个配置库，各配置之间没有任何关系，内存可以分布在各组件内部，达到了完全解耦的作用。
  - 使用这个配置库，可以使用工具方便的修改默认参数，而不用修改代码。
  - 该demo已经在Ubuntu16.04-X64系统上验证通过。

# quick start
  我们以linux系统为例，介绍一下使用和测试方法。

	sudo apt-get install cmake
	cd network
	mkdir build 
	cd build
	cmake ..
	make
	./bin/ffs_demo

# LICENSE
  本网络库开放源码并遵循 [The MIT License](LICENSE) 开源协议。
