## course outline

- relational database

- storage

- execution

- concurrency control

- recovery

- distributed database

- potpourri

# Database Storage(1)

## Agenda

- file storage

- page layout

- tuple layout

## file storage

heap file: page directory

## Page Layout

- Page Header

- Slotted Pages: Page maps slots to offsets

- ~~Log-Structured~~

## Tuple Layout

- Tuple Header

- Tuple Data

- Unique Identifier

- Denormalized Tuple Data

# Database Storage(2)

## Agenda

- Log-Structured Storage

- Index-Organized Storage

- Data Representation

## Log-Structured Storage

- PUT/DELETE operation

- LSM-Tree

- compaction

downsides:

1. write-amplification

2. compaction is expensive

## ideal for write-heavy workloads

but what about read(select)performance

database workloads

- On-Line Transaction Processing(OLTP)

- On-Line Analytical Processing(OLAP)

- Hybrid Transaction  + Analytical Processing

## storage models

1. N-ary Storage Model(NSM)
   
   1. stores all attributes for a single tuple contiguously in a single page.
   
   2. Advantages:
      
      1. fast inserts, updatas, and deletes
      
      2. good for queries that need the entire tuple(OLTP)
      
      3. can use index-oriented physical storage for clustering
   
   3. Disadvantages:
      
      1. not good for scanning large portions of the table and/or a subset of the attributes
      
      2. terrible memory locality in access patterns
      
      3. not ideal for compression because of multyple value domains within a dingle page

2. Decomposition Storage Model(DSM)
   
   1. stores a single attribute for all tuples contiguously in a block of data
   
   2. Advantages:
      
      1. reduves the amount wasted I/O per query bbecause the DBMS only read the data that it needs
      
      2. faster query processing because of increased locality and cached data reuse
      
      3. better data compression
   
   3. Disadvantages:
      
      1. slow for point queries, inserts, updates, and deletes because of tuple splitting/stitching/reorganization

3. Hybrid Storage Model(PAX)
