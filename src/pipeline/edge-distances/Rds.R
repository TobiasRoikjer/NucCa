library(tidyverse)
df <- read.delim(commandArgs(trailingOnly=T)[1], header=F, sep="\t")
df <- sample_n(df, 10000000)
df  %>% arrange(V1, V3) %>% group_by(V1) %>% mutate(dist=V3-lag(V3)) -> df

filter(df, !is.na(dist)) %>% group_by(dist) %>% summarise(n=n()) -> df

write.table(df, file=commandArgs(trailingOnly=T)[2], quote=F,sep="\t", col.names=NA)
