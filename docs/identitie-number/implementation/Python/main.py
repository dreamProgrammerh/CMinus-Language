from idte import Idte

original: int = 0xFE21B3A4D9C8E712
encoded:  str = Idte.encode(original)

print(f"Original: Ox{original:x}")
print(f"Encoded:  {encoded}")

decoded:  int = Idte.decode(encoded)

print(f"Decoded:  Ox{decoded:x}")