# Steganography Image Hiding in C

## 📌 Project Overview

This project implements **Image Steganography** using the **Least Significant Bit (LSB)** technique in the C programming language. It allows users to securely hide a secret text message inside a **24-bit BMP image** and later extract the hidden message without noticeably changing the image.

The project demonstrates concepts such as file handling, bit manipulation, command-line argument processing, and modular programming.

---

## ✨ Features

- Hide a secret text file inside a BMP image.
- Extract the hidden message from the encoded image.
- Uses the Least Significant Bit (LSB) steganography technique.
- Command-line interface for encoding and decoding.
- Modular implementation with separate encoding and decoding modules.
- Written entirely in C.

---

## 🛠 Technologies Used

- Programming Language: C
- Compiler: GCC
- Operating System: Linux / Ubuntu
- Concepts:
  - File Handling
  - Bit Manipulation
  - Structures
  - Command-Line Arguments
  - Image Processing (BMP)

---

## 📁 Project Structure

```text
Steganography-Image-Hiding/
│
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── test_encode.c
├── common.h
├── types.h
├── beautiful.bmp
├── secret.txt
└── README.md
```

---

## ⚙️ Compilation

Compile the project using:

```bash
gcc test_encode.c encode.c decode.c -o stego
```

---

## 🔒 Encoding

Run the following command:

```bash
./stego -e beautiful.bmp secret.txt
```

Example Output:

```text
INFO: ## Encoding Done Successfully ##
```

---

## 🔓 Decoding

Run the following command:

```bash
./stego -d stego.bmp
```

Example Output:

```text
INFO: ## Decoding Done Successfully ##
```

---

## 📖 Algorithm

### Encoding

1. Read the source BMP image.
2. Read the secret text file.
3. Check whether the image has enough capacity.
4. Encode metadata.
5. Hide the secret message using the LSB technique.
6. Save the encoded image.

### Decoding

1. Open the encoded BMP image.
2. Read the metadata.
3. Extract the hidden bits.
4. Reconstruct the original message.
5. Save the decoded text file.

---

## 🎯 Learning Outcomes

This project helped me learn:

- File handling in C
- Bit manipulation
- BMP image format
- Command-line argument processing
- Modular programming
- Steganography fundamentals

---

## 🚀 Future Improvements

- Password-protected encoding
- PNG image support
- Encryption before embedding
- GUI application
- Multiple file support

---

## 👨‍💻 Author

**Mujaheed Ahmed**

GitHub: https://github.com/mujaheed-embedded