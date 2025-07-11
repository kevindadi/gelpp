/*
 * gel::elf::File64 class
 * Copyright (c) 2016, IRIT- université de Toulouse
 *
 * GEL++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GEL++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GEL++; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef GELPP_ELF_FILE64_H_
#define GELPP_ELF_FILE64_H_

#include <elm/data/HashMap.h>
#include <elm/data/Vector.h>
#include <elm/io/RandomAccessStream.h>
#include "../Exception.h"
#include "File.h"
#include "defs64.h"

namespace gel { namespace elf {

class File64;

class ProgramHeader64: public ProgramHeader {
public:
	ProgramHeader64(File *file, Elf64_Phdr *info);

	inline const Elf64_Phdr& info(void) const { return *_info; }

	t::uint32 flags() const override;
	address_t vaddr() const override;
	address_t paddr() const override;
	size_t memsz() const override;
	size_t align() const override;
	int type() const override;
	size_t filesz() const override;
	offset_t offset() const override;

protected:
	t::uint8 *readBuf() override;

private:
	Elf64_Phdr *_info;
};

class Section64: public elf::Section {
public:
	Section64(elf::File64 *file, Elf64_Shdr *entry);
	inline const Elf64_Shdr& info(void) { return *_info; }

	// Section override
	cstring name() override;
	t::uint32 flags() override;
	int type() const override;
	t::uint32 link() const override;
	t::uint64 offset() override;
	address_t addr() const override;
	size_t size() override;
	size_t entsize() const override;
	void read(t::uint8 *buf) override;

	// Segment overload
	address_t baseAddress() override;
	address_t loadAddress() override;
	size_t alignment() override;
	bool isExecutable() override;
	bool isWritable() override;
	bool hasContent() override;
	bool hasContentInFile() override;

	// Section overload
	size_t fileSize() override;

protected:
	t::uint8 *readBuf() override;

private:
	Elf64_Shdr *_info;
};

class File64: public elf::File {
	friend class ProgramHeader64;
	friend class Section64;
public:
	File64(Manager& manager, sys::Path path, io::RandomAccessStream *stream);
	~File64(void);

	const Elf64_Ehdr& info(void) const { return *h; }

	// elf::File overload
	int elfType() override;
	int elfMachine() const override;
	int elfOS() const override;
	t::uint16 version() override;
	const t::uint8 *ident() override;
	void fillSymbolTable(SymbolTable& symtab, Section *sect) override;

	// gel::File overload
	virtual type_t type(void);
	virtual address_type_t addressType(void);
	virtual address_t entry(void);

protected:
	void loadProgramHeaders(Vector<ProgramHeader *>& headers) override;
	void loadSections(Vector<Section *>& sections) override;
	int getStrTab() override;
	void fetchDyn(const t::uint8 *entry, dyn_t& dyn) override;

private:
	Elf64_Ehdr *h;
	t::uint8 *sec_buf;
	t::uint8 *ph_buf;
};

} }	// gel::elf

#endif /* ELF_FILE64_H_ */
