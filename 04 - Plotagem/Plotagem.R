# Bibliotecas
library(ggplot2)
library(dplyr)

# Importação - colocar nome do arquivo
avl <- read.csv("2025-05-13 16.12.42 Plotagem AVL.csv")
bst <- read.csv("2025-05-13 16.12.42 Plotagem BST.csv")

# Adiciona identificadores
avl$tipo <- "AVL"
avl$linha <- 1:nrow(avl)
bst$tipo <- "BST"
bst$linha <- 1:nrow(bst)

dados <- bind_rows(avl, bst)

# Agrupar por janelas de linhas
janela <- 50
dados$grupo <- ceiling(dados$linha / janela)

# Agregações LOCAIS para inserção
resumo_insercao <- dados %>%
  group_by(tipo, grupo) %>%
  summarise(
    x = mean(linha),
    max_y = max(insert_comparisons),
    min_y = min(insert_comparisons),
    media_y = mean(insert_comparisons),
    .groups = "drop"
  )

# Agregações LOCAIS para busca
resumo_busca <- dados %>%
  group_by(tipo, grupo) %>%
  summarise(
    x = mean(linha),
    max_y = max(search_comparisons),
    min_y = min(search_comparisons),
    media_y = mean(search_comparisons),
    .groups = "drop"
  )

# =====================
# GRÁFICO COMPARATIVO - INSERÇÃO
# =====================
ggplot(dados, aes(x = linha, y = insert_comparisons, color = tipo)) +
  geom_point(size = 0.4, alpha = 0.4) +
  geom_line(data = resumo_insercao, aes(x = x, y = max_y, color = tipo), linetype = "dashed", size = 1) +
  geom_line(data = resumo_insercao, aes(x = x, y = media_y, color = tipo), linetype = "solid", size = 1) +
  geom_line(data = resumo_insercao, aes(x = x, y = min_y, color = tipo), linetype = "dotted", size = 1) +
  scale_color_manual(values = c("AVL" = "red", "BST" = "blue")) +
  labs(
    title = "Comparações na Inserção: AVL vs BST",
    x = "Linha",
    y = "Comparações",
    color = "Árvore"
  ) +
  theme_minimal()

# =====================
# GRÁFICO COMPARATIVO - BUSCA
# =====================
ggplot(dados, aes(x = linha, y = search_comparisons, color = tipo)) +
  geom_point(size = 0.4, alpha = 0.4) +
  geom_line(data = resumo_busca, aes(x = x, y = max_y, color = tipo), linetype = "dashed", size = 1) +
  geom_line(data = resumo_busca, aes(x = x, y = media_y, color = tipo), linetype = "solid", size = 1) +
  geom_line(data = resumo_busca, aes(x = x, y = min_y, color = tipo), linetype = "dotted", size = 1) +
  scale_color_manual(values = c("AVL" = "red", "BST" = "blue")) +
  labs(
    title = "Comparações na Busca: AVL vs BST",
    x = "Linha",
    y = "Comparações",
    color = "Árvore"
  ) +
  theme_minimal()

# =====================
# INDIVIDUAL - AVL
# =====================
# Inserção AVL
ggplot(avl, aes(x = linha, y = insert_comparisons)) +
  geom_point(color = "red", size = 0.4, alpha = 0.4) +
  geom_line(data = subset(resumo_insercao, tipo == "AVL"), aes(x = x, y = max_y), color = "firebrick", linetype = "dashed", size = 1) +
  geom_line(data = subset(resumo_insercao, tipo == "AVL"), aes(x = x, y = media_y), color = "darkorange", linetype = "solid", size = 1) +
  geom_line(data = subset(resumo_insercao, tipo == "AVL"), aes(x = x, y = min_y), color = "#FFDAB9", linetype = "dotted", size = 1) +
  labs(title = "AVL - Comparações na Inserção", x = "Linha", y = "Comparações") +
  theme_minimal()

# Busca AVL
ggplot(avl, aes(x = linha, y = search_comparisons)) +
  geom_point(color = "red", size = 0.4, alpha = 0.4) +
  geom_line(data = subset(resumo_busca, tipo == "AVL"), aes(x = x, y = max_y), color = "firebrick", linetype = "dashed", size = 1) +
  geom_line(data = subset(resumo_busca, tipo == "AVL"), aes(x = x, y = media_y), color = "darkorange", linetype = "solid", size = 1) +
  geom_line(data = subset(resumo_busca, tipo == "AVL"), aes(x = x, y = min_y), color = "#FFDAB9", linetype = "dotted", size = 1) +
  labs(title = "AVL - Comparações na Busca", x = "Linha", y = "Comparações") +
  theme_minimal()

# =====================
# INDIVIDUAL - BST
# =====================
# Inserção BST
ggplot(bst, aes(x = linha, y = insert_comparisons)) +
  geom_point(color = "blue", size = 0.4, alpha = 0.4) +
  geom_line(data = subset(resumo_insercao, tipo == "BST"), aes(x = x, y = max_y), color = "#005F73", linetype = "dashed", size = 1) +
  geom_line(data = subset(resumo_insercao, tipo == "BST"), aes(x = x, y = media_y), color = "dodgerblue3", linetype = "solid", size = 1) +
  geom_line(data = subset(resumo_insercao, tipo == "BST"), aes(x = x, y = min_y), color = "skyblue1", linetype = "dotted", size = 1) +
  labs(title = "BST - Comparações na Inserção", x = "Linha", y = "Comparações") +
  theme_minimal()

# Busca BST
ggplot(bst, aes(x = linha, y = search_comparisons)) +
  geom_point(color = "blue", size = 0.4, alpha = 0.4) +
  geom_line(data = subset(resumo_busca, tipo == "BST"), aes(x = x, y = max_y), color = "#005F73", linetype = "dashed", size = 1) +
  geom_line(data = subset(resumo_busca, tipo == "BST"), aes(x = x, y = media_y), color = "dodgerblue3", linetype = "solid", size = 1) +
  geom_line(data = subset(resumo_busca, tipo == "BST"), aes(x = x, y = min_y), color = "skyblue1", linetype = "dotted", size = 1) +
  labs(title = "BST - Comparações na Busca", x = "Linha", y = "Comparações") +
  theme_minimal()
