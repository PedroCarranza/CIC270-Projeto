Esse código utiliza a biblioteca stb_image.

Para que ela funcione corretamente é necessário usar #define STB_IMAGE_IMPLEMENTATION
A biblioteca é utilizada no começo da função initData para carregar a variável img que fornece as informações de altura, largura, quantidade de canais e os parâmetros RGBA utilizando stbi_load. 
Em seguida, a função glTexImage2D carrega as texturas da imagem original, a partir da variável img.

Além disso, como você disse em aula que não era necessário, não foi implementada a alteração da escala no eixo z


