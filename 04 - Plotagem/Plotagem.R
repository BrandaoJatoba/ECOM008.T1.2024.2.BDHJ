# Carregar as bibliotecas necessárias
library(ggplot2)
library(dplyr)

# Importar os dois arquivos CSV
avl <- read.csv("2025-04-27 18.46.41 Plotagem AVL.csv")
bst <- read.csv("2025-04-27 18.46.41 Plotagem BST.csv")

# Adicionar uma coluna identificando a árvore (AVL ou BST) a cada conjunto de dados
avl$tipo <- "AVL"
bst$tipo <- "BST"

# Combinar os dados em um único data frame
dados <- bind_rows(avl, bst)

# ========================
# GRÁFICO 1: Inserção
# ========================
# Calcular média de inserção
media_insercao <- dados %>%
  group_by(tipo) %>%
  summarise(media = mean(insert_comparisons), .groups = "drop")

# Plotar gráfico de inserção
grafico_insercao <- ggplot(dados, aes(x = number, y = insert_comparisons, color = tipo)) +
  geom_point(size = 0.7) +
  geom_smooth(method = "loess", se = FALSE, linetype = "dotted", size = 2) +
  labs(
    title = "Comparações na Inserção: AVL vs BST",
    x = "Número de elementos",
    y = "Número de comparações",
    color = "Árvore"
  ) +
  scale_color_manual(values = c("AVL" = "red", "BST" = "blue")) +
  theme_minimal()

# Mostrar o gráfico
print(grafico_insercao)

# ========================
# GRÁFICO 2: Busca
# ========================
# Calcular média de busca
media_busca <- dados %>%
  group_by(tipo) %>%
  summarise(media = mean(search_comparisons), .groups = "drop")

# Plotar gráfico de busca
grafico_busca <- ggplot(dados, aes(x = number, y = search_comparisons, color = tipo)) +
  geom_point(size = 0.7) +
  geom_smooth(method = "loess", se = FALSE, linetype = "dotted", size = 2) +
  labs(
    title = "Comparações na Busca: AVL vs BST",
    x = "Número de elementos",
    y = "Número de comparações",
    color = "Árvore"
  ) +
  scale_color_manual(values = c("AVL" = "red", "BST" = "blue")) +
  theme_minimal()

# Mostrar o gráfico
print(grafico_busca)
