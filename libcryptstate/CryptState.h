/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _CRYPTSTATE_H
#define _CRYPTSTATE_H

#include <openssl/aes.h>

namespace MumbleClient {

class CryptState {
	private:
		unsigned char raw_key[AES_BLOCK_SIZE];
		unsigned char encrypt_iv[AES_BLOCK_SIZE];
		unsigned char decrypt_iv[AES_BLOCK_SIZE];
		unsigned char decrypt_history[0x100];

		unsigned int uiGood;
		unsigned int uiLate;
		unsigned int uiLost;
		unsigned int uiResync;

		unsigned int uiRemoteGood;
		unsigned int uiRemoteLate;
		unsigned int uiRemoteLost;
		unsigned int uiRemoteResync;

		AES_KEY encrypt_key;
		AES_KEY decrypt_key;
		bool bInit;

	public:
		CryptState();

		 bool isValid() const;
		 void genKey();
		 void setKey(const unsigned char* rkey, const unsigned char* eiv, const unsigned char* div);
		 void setDecryptIV(const unsigned char* iv);
		 const unsigned char* getEncryptIV() const;

		void ocb_encrypt(const unsigned char* plain, unsigned char* encrypted, unsigned int len, const unsigned char* nonce, unsigned char* tag);
		void ocb_decrypt(const unsigned char* encrypted, unsigned char* plain, unsigned int len, const unsigned char* nonce, unsigned char* tag);

		 bool decrypt(const unsigned char* source, unsigned char* dst, unsigned int crypted_length);
		 void encrypt(const unsigned char* source, unsigned char* dst, unsigned int plain_length);
};

extern "C" {
  CryptState* CryptState_new() { return new CryptState(); }
  bool CryptState_decrypt(CryptState* cs, const unsigned char* source, unsigned char* dst, unsigned int crypted_length) { return cs->decrypt(source, dst, crypted_length); }
  void CryptState_encrypt(CryptState* cs, const unsigned char* source, unsigned char* dst, unsigned int plain_length) { cs->encrypt(source, dst, plain_length); }
  void CryptState_setKey(CryptState* cs, const unsigned char* rkey, const unsigned char* eiv, const unsigned char* div) { cs->setKey(rkey, eiv, div); }
  void CryptState_setDecryptIV(CryptState* cs, const unsigned char* iv) { cs->setDecryptIV(iv); }
  bool CryptState_isValid(CryptState* cs) { return cs->isValid(); }
}

}  // namespace MumbleClient

#endif
