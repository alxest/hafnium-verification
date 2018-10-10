/*
 * Copyright 2018 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdalign.h>
#include <stdint.h>

#include "hf/mm.h"
#include "hf/std.h"

#include "vmapi/hf/call.h"

#define FORWARD_VM_ID 2

alignas(4096) uint8_t kstack[4096];

static alignas(PAGE_SIZE) uint8_t send_page[PAGE_SIZE];
static alignas(PAGE_SIZE) uint8_t recv_page[PAGE_SIZE];

static hf_ipaddr_t send_page_addr = (hf_ipaddr_t)send_page;
static hf_ipaddr_t recv_page_addr = (hf_ipaddr_t)recv_page;

void kmain(void)
{
	hf_vm_configure(send_page_addr, recv_page_addr);
	hf_vm_configure(send_page_addr, recv_page_addr);

	/* Loop, forward messages to the next VM. */
	for (;;) {
		struct hf_mailbox_receive_return res = hf_mailbox_receive(true);
		memcpy(send_page, recv_page, res.size);
		hf_mailbox_clear();
		hf_mailbox_send(FORWARD_VM_ID, res.size);
	}
}
