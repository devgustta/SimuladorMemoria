/*
 * simuladorMemoria.c
 * 
 * Copyright 2025 Gustavo <Gustavo@DESKTOP-2IA8145>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>


typedef struct block{
    int size;
    int is_free;
    struct block* nxt;
}BlockHEader;


void* alloc(int size,unsigned char *mem, int *freeMem){
	
	int offset = 0;
	
	while(offset < 1024){ // trasnforma aque pedaço do array ( mem + offset) em uma struct
		BlockHEader* header = (BlockHEader*) (mem + offset);
		
		
		if(header->is_free == 1 && header->size > size ){
			int total_block_size = sizeof(BlockHEader) + size;
			//int remaining_mem = header->size - size - sizeof(BlockHEader);
			
			
			header->size = size;
			header->is_free = 0; // 0 ocupado, 1 livre
			*freeMem -= total_block_size;
			
			if(total_block_size < 1024){ // criando o proximo cabeçalho se tivermos memoria, com isso sabemos tambem quanto tem de memoria disponivel
				BlockHEader* next = (BlockHEader*)((unsigned char*)(header+1) + size);
				next->size = *freeMem;                        // header + 1 é como se tivessemos pulando o cabeçalho e indo para area de dados
				next->is_free = 1;
				next->nxt = NULL;
				header->nxt = next;
			}
			
			return (void*)((unsigned char*)header + sizeof(BlockHEader));
		}
		// esse é o cara que nos auxilia a andar pelo nosso array(memoria simulada) somando o heade->size atual mas o tamanho do cabeçalho
		offset += header->size + sizeof(BlockHEader);
	}
	return NULL;
}


void joinBlock(void *mem){
		
	BlockHEader* header = (BlockHEader*) mem;
	while(header != NULL && header->nxt != NULL){
		if(header->is_free && header->nxt->is_free){
			header->size += header->nxt->size + sizeof(BlockHEader);
			header->nxt = header->nxt->nxt;
			
		}else{
				header = header->nxt;
		}	
	}
}

void freeMemory(unsigned char* ptr, int *freeMem){
	BlockHEader* header = (BlockHEader*)(ptr - sizeof(BlockHEader));
	header->is_free = 1;
	if(header->is_free == 1){
		*freeMem += sizeof(BlockHEader) + header->size;
    }
    joinBlock(ptr);
}

int showMemory(void *mem){
	int offset = 0;
	
	while(offset < 1024){
		
		BlockHEader* header = (BlockHEader*)(mem+offset);
		if(header->is_free == 0){
			printf("***************\n");
			printf("Tamanho do Bloco: %d, Estado do Bloco: %s\n", header->size, header->is_free ? "LIVRE": "OCUPADO");
			
		}
		offset += sizeof(BlockHEader)+ header->size;
	}
	
	return 0;
}




int main(int argc, char **argv)
{
	int ram = 1024;
	int *freeMem = &ram;
	//int size = 10;
	unsigned char mem[1024] = {0};
	
	BlockHEader* firstBlock = (BlockHEader*) mem;
	firstBlock->size = *freeMem - sizeof(BlockHEader);
	firstBlock->is_free = 1; // primeiro bloco criado como livre
	
	void* p1;
	void* p2;
	
	
	while(*freeMem > 0){
		int op;
		printf("1 - Alocar Memoria\n");
		printf("2 - Liberar Memoria\n");
		printf("3 - Mostrar Memoria Alocada\n");
		scanf("%d", &op);
		
		switch(op){
			case 1:
				p1 = alloc(10, mem, freeMem);
				printf("Memoria Disponivel: %d\n", *freeMem);
				p2 = alloc(20, mem, freeMem);
				printf("Memoria Disponivel: %d\n", *freeMem);
				break;
			case 2:
				freeMemory(p2, freeMem);
				printf("Memoria Disponivel: %d\n", *freeMem);
				freeMemory(p1, freeMem);
				printf("Memoria Disponivel: %d\n", *freeMem);
				
				break;
			case 3:
				showMemory(mem);
				break;
		}
	}
	
	return 0;
}

