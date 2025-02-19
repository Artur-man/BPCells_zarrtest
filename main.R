library(dplyr)
library(ggplot2)
options(scipen = 999)
datax <- read.table("test_all.txt", sep = " ", header = FALSE, colClasses = c("numeric", "factor", "factor", "numeric"))
colnames(datax) <- c("shape", "chunk", "append.size", "time")
datax$iter <- rep(1:(nrow(datax)/5), each = 5)
datax[,c("shape")] <- datax[,c("shape")]/10000
datax[,c("chunk")] <- gsub("0000$", "", datax[,c("chunk")])
datax[,c("append.size")] <- gsub("0000$", "", datax[,c("append.size")])
datax <- datax %>%
  group_by(iter) %>%
  mutate(time = mean(time)) %>%
  select(-iter) %>%
  distinct()
temp <- factor(datax$chunk)
levels(temp) <- as.character(sort(unique(as.numeric(as.character(temp)))))
datax$chunk <- temp
temp <- factor(datax$append.size)
levels(temp) <- as.character(sort(unique(as.numeric(as.character(temp)))))
datax$append.size <- temp
ggplot(data=datax, aes(x=shape, y = time, color = chunk)) +
  geom_line() +
  facet_wrap(~append.size) +
  ylab("time (sec)") +
  xlab("n of elements (*10000)") +
  labs(title = "Title change in time of append by append size (*10000)") +
  guides(color=guide_legend(title="Chunk size (*10000)"))
