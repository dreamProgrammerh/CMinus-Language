class Idte:
    _BASE = 64
    _FIXED_LENGTH = 11
    _alphabet: str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_$"
    _table: list[int] = [
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, 63, -1, -1, -1,  # '$' = 63
        -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  # '0'–'9' = 0–9
         2,  3,  4,  5,  6,  7,  8,  9, -1, -1,
        -1, -1, -1, -1, -1, 36, 37, 38, 39, 40,  # 'A'–'Z' = 36–61
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
        51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
        61, -1, -1, -1, -1, 62, -1, 10, 11, 12,  # '_' = 62, 'a'–'z' = 10–35
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
        23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, -1, -1, -1, -1, -1
    ]
    
    @staticmethod
    def encodeHash(value: int) -> str:
        # Encodes a 64-bit integer into a fixed-length 11-character Identitie string.
        chars: list[str] = [''] * Idte._FIXED_LENGTH
        for i in range(Idte._FIXED_LENGTH - 1, -1, -1):
            chars[i] = Idte._alphabet[value & 0x3F]
            value >>= 6

        return ''.join(chars)

    @staticmethod
    def decodeHash(s: str) -> int:
        # Decodes a fixed-length 11-character Identitie string into a 64-bit integer.
        if len(s) != Idte._FIXED_LENGTH:
            raise ValueError("Invalid Identitie hash length")

        value = 0
        for c in s:
            if c not in Idte._table:
                raise ValueError(f"Invalid character in Identitie string: {c}")

            value = (value << 6) | Idte._table[c]
        return value


    @staticmethod
    def encode(value: int) -> str:
        # Encodes an integer into a variable-length Identitie string.
        if value == 0:
            return Idte._alphabet[0]
            
        chars = []
        
        while value > 0:
            chars.append(Idte._alphabet[value % Idte._BASE])
            value //= Idte._BASE
            
        return ''.join(reversed(chars))

    @staticmethod
    def decode(s: str) -> int:
        # Decodes a variable-length Identitie string into an integer.
        value = 0
        
        for c in s:
            code = ord(c)
            if code >= len(Idte._table) or Idte._table[code] == -1:
                raise ValueError(f"Invalid character in Identitie string: {c}")
            
            value = value * Idte._BASE + Idte._table[code]
        
        return value
        
