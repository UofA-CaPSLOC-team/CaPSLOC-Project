
-- --------------------------------------------------
-- Entity Designer DDL Script for SQL Server 2005, 2008, and Azure
-- --------------------------------------------------
-- Date Created: 02/20/2014 15:00:02
-- Generated from EDMX file: C:\src\CaPSLOC-Project\CTRL\CaPSLOC\DbModel.edmx
-- --------------------------------------------------

SET QUOTED_IDENTIFIER OFF;
GO
USE [CaPSLOC];
GO
IF SCHEMA_ID(N'dbo') IS NULL EXECUTE(N'CREATE SCHEMA [dbo]');
GO

-- --------------------------------------------------
-- Dropping existing FOREIGN KEY constraints
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[FK_LocationData]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Data] DROP CONSTRAINT [FK_LocationData];
GO
IF OBJECT_ID(N'[dbo].[FK_ALTData]', 'F') IS NOT NULL
    ALTER TABLE [dbo].[Data] DROP CONSTRAINT [FK_ALTData];
GO

-- --------------------------------------------------
-- Dropping existing tables
-- --------------------------------------------------

IF OBJECT_ID(N'[dbo].[Locations]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Locations];
GO
IF OBJECT_ID(N'[dbo].[Scripts]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Scripts];
GO
IF OBJECT_ID(N'[dbo].[ALTs]', 'U') IS NOT NULL
    DROP TABLE [dbo].[ALTs];
GO
IF OBJECT_ID(N'[dbo].[Data]', 'U') IS NOT NULL
    DROP TABLE [dbo].[Data];
GO
IF OBJECT_ID(N'[dbo].[AppSettings]', 'U') IS NOT NULL
    DROP TABLE [dbo].[AppSettings];
GO

-- --------------------------------------------------
-- Creating all tables
-- --------------------------------------------------

-- Creating table 'Locations'
CREATE TABLE [dbo].[Locations] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL,
    [Latitude] float  NOT NULL,
    [Longitude] float  NOT NULL,
    [Altitude] float  NOT NULL
);
GO

-- Creating table 'Scripts'
CREATE TABLE [dbo].[Scripts] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL,
    [Description] nvarchar(max)  NOT NULL,
    [Content] nvarchar(max)  NOT NULL,
    [Shared] bit  NOT NULL
);
GO

-- Creating table 'ALTs'
CREATE TABLE [dbo].[ALTs] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [Name] nvarchar(max)  NOT NULL,
    [Address] nvarchar(max)  NOT NULL,
    [RecentlyLocated] bit  NOT NULL
);
GO

-- Creating table 'Data'
CREATE TABLE [dbo].[Data] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [ImageFilename] nvarchar(300)  NOT NULL,
    [ImageEncoding] nvarchar(10)  NOT NULL,
    [CaptureTime] datetime  NOT NULL,
    [LocationId] int  NOT NULL,
    [ALTId] int  NOT NULL
);
GO

-- Creating table 'AppSettings'
CREATE TABLE [dbo].[AppSettings] (
    [Id] int IDENTITY(1,1) NOT NULL,
    [DisplayName] nvarchar(max)  NOT NULL,
    [ShortName] nvarchar(max)  NOT NULL,
    [Value] nvarchar(max)  NOT NULL
);
GO

-- --------------------------------------------------
-- Creating all PRIMARY KEY constraints
-- --------------------------------------------------

-- Creating primary key on [Id] in table 'Locations'
ALTER TABLE [dbo].[Locations]
ADD CONSTRAINT [PK_Locations]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Scripts'
ALTER TABLE [dbo].[Scripts]
ADD CONSTRAINT [PK_Scripts]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'ALTs'
ALTER TABLE [dbo].[ALTs]
ADD CONSTRAINT [PK_ALTs]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'Data'
ALTER TABLE [dbo].[Data]
ADD CONSTRAINT [PK_Data]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- Creating primary key on [Id] in table 'AppSettings'
ALTER TABLE [dbo].[AppSettings]
ADD CONSTRAINT [PK_AppSettings]
    PRIMARY KEY CLUSTERED ([Id] ASC);
GO

-- --------------------------------------------------
-- Creating all FOREIGN KEY constraints
-- --------------------------------------------------

-- Creating foreign key on [LocationId] in table 'Data'
ALTER TABLE [dbo].[Data]
ADD CONSTRAINT [FK_LocationData]
    FOREIGN KEY ([LocationId])
    REFERENCES [dbo].[Locations]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;

-- Creating non-clustered index for FOREIGN KEY 'FK_LocationData'
CREATE INDEX [IX_FK_LocationData]
ON [dbo].[Data]
    ([LocationId]);
GO

-- Creating foreign key on [ALTId] in table 'Data'
ALTER TABLE [dbo].[Data]
ADD CONSTRAINT [FK_ALTData]
    FOREIGN KEY ([ALTId])
    REFERENCES [dbo].[ALTs]
        ([Id])
    ON DELETE NO ACTION ON UPDATE NO ACTION;

-- Creating non-clustered index for FOREIGN KEY 'FK_ALTData'
CREATE INDEX [IX_FK_ALTData]
ON [dbo].[Data]
    ([ALTId]);
GO

-- --------------------------------------------------
-- Script has ended
-- --------------------------------------------------